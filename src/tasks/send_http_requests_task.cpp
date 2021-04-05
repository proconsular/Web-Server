//
// Created by Chris Luttio on 3/31/21.
//

#include "send_http_requests_task.h"

void SendHTTPRequestsTask::perform() {
    for (auto envelope: _state->outbound_http_request_queue) {
        if (envelope->status == CONNECTED) {
            auto data = envelope->http_request->generate();
            envelope->connection->write(data);
            envelope->status = SENT;
        }
    }
}