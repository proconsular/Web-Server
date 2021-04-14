//
// Created by Chris Luttio on 3/31/21.
//

#include "send_http_requests_task.h"

void SendHTTPRequestsTask::perform() {
    for (const auto& pair: _state->outbound_http_request_queue) {
        auto envelope = std::make_shared<HTTPRequestCarrier>(*pair.second);
        if (envelope->status == CONNECTED) {
            auto data = envelope->http_request->generate();
            envelope->connection->write(data);
            envelope->status = SENT;
            _controller->apply(Action(SendHttpRequest, envelope));
        }
    }
}