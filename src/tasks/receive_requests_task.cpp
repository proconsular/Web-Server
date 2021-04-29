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

            const int buffer_size = 10 * 1024;

            auto last_read = get_time();

            HttpMessageParser parser(REQUEST);

            bool still_reading = false;
            bool read_any = false;

            int data_read;
            do {
                char *buffer = new char[buffer_size];

                if (connection->security == UNSECURE) {
                    data_read = read(connection->socket.id, buffer, buffer_size);
                } else {
                    data_read = SSL_read(connection->ssl, buffer, buffer_size);
                }

                if (data_read > 0) {
                    last_read = get_time();
                    still_reading = parser.partial_parse(buffer, data_read);
                    read_any = true;
                }

                delete[] buffer;
            } while (get_ms_to_now(last_read) <= 1000 && still_reading);

            if (read_any) {
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