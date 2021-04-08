//
// Created by Chris Luttio on 3/27/21.
//

#include "process_http_requests_task.h"

void ProcessHTTPRequestsTask::perform() {
    for (auto envelope: state->inbound_http_request_queue) {
        auto request = envelope.request;
        auto client_request = std::make_shared<ClientRequest>();
        client_request->connection = envelope.connection;
        if (request->method == "GET") {
            client_request->type = Requests::RetrieveFile;
            client_request->uri = request->uri;
        }
        _controller->apply(Action(CreateClientRequest, client_request));
    }
    state->inbound_http_request_queue.clear();
}