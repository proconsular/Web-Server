//
// Created by Chris Luttio on 3/25/21.
//

#include "receive_requests_task.h"
#include "http_request.h"
#include "http_request_parser.h"

void ReceiveRequestsTask::perform() {
    for (auto connection : state->connections) {
        int error = connection->socket.get_error();
        if (error == 0) {
            auto* input = new std::string;
            if (connection->read(input)) {
                HTTPRequest* request = HTTPRequestParser::parse(*input);
                state->http_request_queue.emplace_back(connection, request);
                connection->active_requests++;
            }
            delete input;
        } else {
            connection->terminate();
        }
    }
}