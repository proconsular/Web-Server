//
// Created by Chris Luttio on 3/25/21.
//

#include "receive_requests_task.h"
#include "http_request.h"
#include "http_request_parser.h"
#include "http_response.h"
#include <iostream>

void ReceiveRequestsTask::perform() {
    for (int i = 0; i < state->connections.size(); i++) {
        auto connection = state->connections[i];

        int error_code;
        int error_code_size = sizeof(error_code);
        getsockopt(connection->socket.id(), SOL_SOCKET, SO_ERROR, &error_code, reinterpret_cast<socklen_t *>(&error_code_size));

        if (error_code == 0) {
            char buffer[1024] = {0};
            int amount = read(connection->socket.id(), buffer, 1024);

            if (amount > 0) {
                connection->last_read = std::chrono::high_resolution_clock::now();
            }

            if (strlen(buffer) > 0) {
                HTTPRequest* request = HTTPRequestParser::parse(buffer);
                std::cout << request->method << ": " << request->uri << std::endl;

                HTTPResponse response = HTTPResponse::OK();
                response.body = "Hello world";

                std::string message = response.generate();
                send(connection->socket.id(), message.c_str(), message.length(), 0);
                std::cout << response.code << ": " << response.status << std::endl;
            }
        } else {
            connection->alive = false;
        }
    }
}