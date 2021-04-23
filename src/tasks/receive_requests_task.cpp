//
// Created by Chris Luttio on 3/25/21.
//

#include "receive_requests_task.h"
#include "http_message_parser.h"

void ReceiveRequestsTask::perform() {
    for (const auto& pair : state->connections) {
        auto connection = std::make_shared<Connection>(*pair.second);
        if (!connection->alive)
            continue;
        int error = connection->socket.get_error();
        if (error == 0) {
            auto input = std::make_shared<std::string>();
            if (connection->read(input)) {
                HttpMessageParser parser(REQUEST);
                parser.partial_parse(input->c_str(), input->size());
                parser.finalize();
                connection->active_requests++;
                connection->last_read = std::chrono::high_resolution_clock::now();
                _controller->apply(Action(ModifyClientConnection, connection));
                _controller->apply(Action(CreateHttpRequest, std::make_shared<HTTPRequestEnvelope>(connection, parser.get_message())));
            }
        } else {
            connection->terminate();
        }
    }
}