//
// Created by Chris Luttio on 3/25/21.
//

#include <constants.h>
#include "receive_requests_task.h"
#include "http_message_parser.h"
#include "http_request_reader.h"

void ReceiveRequestsTask::perform() {
    for (const auto& pair : state->connections) {
        auto connection = std::make_shared<Connection>(*pair.second);
        if (!connection->alive)
            continue;
        int error = connection->socket.get_error();
        if (error == 0) {
            HttpRequestReader reader(10 * KB);
            int result = reader.read(connection);
            if (result != 0) {
                connection->active_requests++;
                connection->last_read = std::chrono::high_resolution_clock::now();
                if (result == 1) {
                    _controller->apply(Action(CreateHttpRequest, std::make_shared<HTTPRequestEnvelope>(connection, reader.get_message())));
                } else {
                    auto client_request = std::make_shared<ClientRequest>();
                    client_request->connection = connection;
                    client_request->type = BadRequest;
                    client_request->status = Complete;
                    connection->persistence = Connection::CLOSE;

                    _controller->apply(Action(CreateClientRequest, client_request));
                }
                _controller->apply(Action(ModifyClientConnection, connection));
            }
        } else {
            connection->terminate();
        }
    }
}