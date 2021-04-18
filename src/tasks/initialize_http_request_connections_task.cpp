//
// Created by Chris Luttio on 4/1/21.
//

#include "initialize_http_request_connections_task.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/un.h>
#include <arpa/inet.h>

void InitializeHTTPRequestConnectionsTask::perform() {
    for (const auto& pair: _state->outbound_http_request_queue) {
        auto carrier = pair.second;
        if (carrier->status == NEW) {
            auto _carrier = std::make_shared<HTTPRequestCarrier>(*carrier);

            int socket_id = -1;

            if (carrier->url.protocol == "http") {
                socket_id = create_ip_socket(carrier);
            } else if (carrier->url.protocol == "unix") {
                socket_id = create_unix_socket(carrier);
            }

            if (socket_id >= 0) {
                auto connection = std::make_shared<Connection>(Socket(socket_id));
                _carrier->connection = connection;
                _carrier->status = CONNECTED;
            } else {
                _carrier->status = FAILED;
            }

            _controller->apply(Action(InitializeHttpRequestConnection, _carrier));
        }
    }
}

int InitializeHTTPRequestConnectionsTask::create_ip_socket(const std::shared_ptr<HTTPRequestCarrier> &carrier) {
    sockaddr_in server_address{};

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(80);
    if (carrier->url.port != 0) {
        server_address.sin_port = htons(carrier->url.port);
    }

    struct addrinfo hints{}, *res, *res0;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int socket_id = -1;

    const char* domain = carrier->url.domain_to_cstr();
    if (strcmp("localhost", domain) == 0) {
        socket_id = socket(AF_INET, SOCK_STREAM, 0);

        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

        if (connect(socket_id, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
            close(socket_id);
            socket_id = -1;
        }
    } else {
        int error = getaddrinfo(domain, carrier->url.protocol.c_str(), &hints, &res0);
        delete domain;

        if (error) {
            return -1;
        }

        for (res = res0; res; res = res->ai_next) {
            socket_id = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (socket_id < 0) {
                continue;
            }
            if (connect(socket_id, res->ai_addr, res->ai_addrlen) == -1) {
                close(socket_id);
                socket_id = -1;
                continue;
            }
            break;
        }
        freeaddrinfo(res0);
    }

    return socket_id;
}

int InitializeHTTPRequestConnectionsTask::create_unix_socket(const std::shared_ptr<HTTPRequestCarrier> &carrier) {
    int sock = socket(AF_UNIX, SOCK_STREAM, 0);

    auto str = carrier->url.to_string().substr(7);

    struct sockaddr_un addr{};
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, str.c_str(), str.size());

    int error = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (error < 0) {
        return -1;
    }

    return sock;
}