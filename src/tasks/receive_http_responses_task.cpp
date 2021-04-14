//
// Created by Chris Luttio on 4/1/21.
//

#include "receive_http_responses_task.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

enum ReadMode {
    STATUS,
    HEADERS,
    BODY,
    CHUNKED,
};

void ReceiveHTTPResponsesTask::perform() {
    for (const auto& pair: _state->outbound_http_request_queue) {
        auto carrier = std::make_shared<HTTPRequestCarrier>(*pair.second);
        if (carrier->status == SENT) {

            const ssize_t buffer_size = 1024 * 4;

            auto response = std::make_shared<HTTPResponse>();
            auto data = std::make_shared<std::string>();
            auto accum = std::make_shared<std::string>();

            int socket = carrier->connection->socket.id();

            ssize_t amount = 0;
            bool reading = true;

            int cursor = 0;
            int body_length = 0;

            ReadMode mode = STATUS;

            do {
                char* buffer = new char[buffer_size];

                amount = read(socket, buffer, buffer_size);

                if (amount == -1)
                    break;

                data->append(std::string(buffer, buffer + amount));

                bool parsing = true;
                while (cursor < data->size() && parsing) {
                    switch (mode) {
                        case STATUS: {
                            cursor += HTTPResponse::parse_status_line(data->begin(), data->end(), response);
                            mode = HEADERS;
                            break;
                        }
                        case HEADERS: {
                            int length = HTTPResponse::parse_header(data->begin() + cursor, data->end(), response);
                            if (length == -1) {
                                parsing = false;
                                break;
                            }
                            cursor += length;
                            if (cursor < data->size()) {
                                if (data->at(cursor) == '\r' || data->at(cursor) == '\n') {
                                    if (response->headers.find("Content-Length") != response->headers.end()) {
                                        body_length = atoi(response->headers["Content-Length"]->c_str());
                                        mode = BODY;
                                    } else if (response->headers.find("Transfer-Encoding") != response->headers.end()) {
                                        mode = CHUNKED;
                                    }
                                    if (data->at(cursor) == '\r') {
                                        cursor += 2;
                                    } else {
                                        cursor += 1;
                                    }
                                }
                            }
                            break;
                        }
                        case CHUNKED: {
                            int size;
                            int length = HTTPResponse::parse_chunk_header(data, cursor, size);
                            if (length == -1) {
                                parsing = false;
                                break;
                            }
                            if (size == 0) {
                                parsing = false;
                                reading = false;
                                break;
                            }
                            if (cursor + length + size >= data->size()) {
                                parsing = false;
                                break;
                            }
                            cursor += length;
                            accum->append(std::string(data->begin() + cursor, data->begin() + cursor + size));
                            cursor += size + 2;
                            break;
                        }
                        default:
                            parsing = false;
                            break;
                    }
                }

                if (mode == BODY && cursor + body_length == data->size()) {
                    response->body = std::make_shared<std::string>(data->begin() + cursor, data->begin() + cursor + body_length);
                    break;
                }

                delete[] buffer;
            } while (amount > 0 && reading);

            if (!accum->empty()) {
                response->body = accum;
            }

            carrier->status = FULFILLED;

            carrier->http_response = response;
//            _controller->apply(Action(ReportLog, response->generate()));
            _controller->apply(Action(ReceiveHttpResponse, carrier));
        }
    }
}