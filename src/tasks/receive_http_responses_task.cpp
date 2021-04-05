//
// Created by Chris Luttio on 4/1/21.
//

#include "receive_http_responses_task.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

void ReceiveHTTPResponsesTask::perform() {
    for (auto carrier: _state->outbound_http_request_queue) {
        if (carrier->status == SENT) {
            auto* response = new HTTPResponse;

            int socket = carrier->connection->socket.id();
            int buffer_size = 1024 * 10;
            int amount_remaining = -1;
            int amount_read = 0;
            int cursor = 0;
            int last_size = -1;

            auto* accum = new std::string;

            bool parsed_header = false;
            bool chunked = false;
            do {
                char* buffer = new char[buffer_size];
                ssize_t amount = read(socket, buffer, buffer_size);
                std::cout << "reading\n";
                if (amount >= 0) {
                    if (!parsed_header) {
                        parsed_header = true;
                        int header_end = HTTPResponse::parse_header(buffer, buffer + buffer_size, response);
                        accum->append(std::string(buffer + header_end, buffer + amount));
                        cursor = 0;
                        if (response->headers.find("Content-Length") != response->headers.end()) {
                            int length = atoi(response->headers["Content-Length"].c_str());
                            if (header_end + length < buffer_size) {
                                HTTPResponse::parse_body(buffer + header_end, response);
                                break;
                            } else {
                                amount_remaining = (header_end + length) - buffer_size;
                            }
                        } else {
                            chunked = true;
                        }
                    } else {
                        if (!chunked) {
                            amount_remaining -= amount;
                            accum->append(std::string(buffer, buffer + amount));
                            if (amount_remaining <= 0) {
                                response->body = accum;
                                break;
                            }
                        } else {
                            accum->append(std::string(buffer, buffer + amount));

                            int size = last_size;
                            do {
                                std::cout << cursor << std::endl;
                                if (size == -1) {
                                    int result = HTTPResponse::parse_chunk_header(*accum, cursor, size);
                                    if (result <= -1)
                                        break;
                                    cursor = result;
                                    last_size = -1;
                                }
                                if (size == 0 || size == -1)
                                    break;
                                if (cursor + size < accum->size()) {
                                    cursor = HTTPResponse::parse_chunk(*accum, cursor, size, response);
                                    size = -1;
                                } else {
                                    last_size = size;
                                    break;
                                }
                            } while (cursor < accum->size());

                            if (size == 0) {
                                break;
                            }
                        }
                    }
                    delete[] buffer;
                } else {
                    break;
                }
            } while(true);
            std::cout << *response->body << std::endl;
            FILE* file = fopen("test.html", "w");
            fwrite(response->body->c_str(), sizeof(char), response->body->size(), file);
            fclose(file);
            carrier->status = FULFILLED;
        }
    }
}