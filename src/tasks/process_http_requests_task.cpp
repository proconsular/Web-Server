//
// Created by Chris Luttio on 3/27/21.
//

#include "process_http_requests_task.h"

#include <algorithm>

void ProcessHTTPRequestsTask::perform() {
    for (const auto& envelope: state->inbound_http_request_queue) {
        auto request = envelope.request;
        auto client_request = std::make_shared<ClientRequest>();
        client_request->connection = envelope.connection;

        auto connection_header = request->headers.find("Connection");
        if (connection_header != request->headers.end()) {
            std::string value(*connection_header->second);
            std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return std::tolower(c); });
            if (value == "close") {
                envelope.connection->persistence = Connection::CLOSE;
            }
        }

        if (request->method == "GET") {
            if (state->routes.empty() || state->routes.find(request->url.to_string()) == state->routes.end()) {
                client_request->type = Requests::RetrieveFile;
                client_request->uri = request->url;
            } else {
                client_request->type = ResolveRoute;
                client_request->route = state->routes[request->url.to_string()];
                client_request->http_request = request;
            }
        }
        _controller->apply(Action(CreateClientRequest, client_request));
    }
    _controller->apply(Action(ClearHttpRequests));
}