//
// Created by Chris Luttio on 3/27/21.
//

#include "process_http_requests_task.h"

void ProcessHTTPRequestsTask::perform() {
    for (auto envelope: state->http_request_queue) {
        auto request = envelope.request;
        auto* client_request = new ClientRequest;
        client_request->connection = envelope.connection;
        if (request->method == "GET") {
            client_request->type = Requests::RetrieveFile;
            client_request->uri = request->uri;
            std::cout << "INFO: Retrieve File Request CREATED" << std::endl;
        }
        state->requests.push_back(client_request);
    }
    state->http_request_queue.clear();
}