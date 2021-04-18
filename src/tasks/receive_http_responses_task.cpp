//
// Created by Chris Luttio on 4/1/21.
//

#include "receive_http_responses_task.h"
#include "http_response_parser.h"
#include "constants.h"
#include "utils.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>

void ReceiveHTTPResponsesTask::perform() {
    for (const auto& pair: _state->outbound_http_request_queue) {
        auto carrier = std::make_shared<HTTPRequestCarrier>(*pair.second);
        if (carrier->status == SENT) {
            if (!carrier->has_been_read) {
                carrier->initial_read_time = get_time();
                carrier->has_been_read = true;
            }

            const ssize_t buffer_size = 100 * KB;

            int socket = carrier->connection->socket.id();

            ssize_t amount = 0;

            HttpResponseParser parser;

            auto was_read_time = get_time();

            bool did_read_any = false;

            do {
                char* buffer = new char[buffer_size];

                amount = read(socket, buffer, buffer_size);

                if (amount > 0) {
                    was_read_time = get_time();
                    did_read_any = true;
                    if (!parser.partial_parse(buffer, amount)) {
                        carrier->status = FULFILLED;
                        delete[] buffer;
                        break;
                    }
                } else {
                    if (get_ms_to_now(was_read_time) >= _state->config->http_response_read_wait_ms) {
                        delete[] buffer;
                        parser.finalize();
                        break;
                    }
                }

                delete[] buffer;
            } while (true);

            if (did_read_any) {
                carrier->status = FULFILLED;
                carrier->http_response = parser.get_response();
            } else {
                if (get_ms_to_now(carrier->initial_read_time) >= 15000) {
                    carrier->status = FAILED;
                }
            }
            
            _controller->apply(Action(ReceiveHttpResponse, carrier));
        }
    }
}
