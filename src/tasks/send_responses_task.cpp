//
// Created by Chris Luttio on 3/27/21.
//

#include "send_responses_task.h"

void SendResponsesTask::perform() {
    for (auto envelope: state->outbound_http_response_queue) {
        auto response = envelope.response->generate();
        envelope.connection->socket.write(response);
//        envelope.connection->terminate();
    }
    if (!state->outbound_http_response_queue.empty()) {
        _controller->apply(Action(ClearHttpResponses));
    }
}