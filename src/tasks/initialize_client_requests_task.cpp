//
// Created by Chris Luttio on 3/27/21.
//

#include "initialize_client_requests_task.h"
#include "load_requested_file_task.h"

void InitializeClientRequestsTask::perform() {
    for (const auto& pair: state->requests) {
        auto request = new ClientRequest(*pair.second);
        if (request->status == RequestStatus::New) {
            switch (request->type) {
                case RetrieveFile: {
                    state->scheduler->add(new LoadRequestedFileTask(_controller, request, state->config));
                    request->status = RequestStatus::Working;
                    _controller->apply(Action(ModifyClientRequest, request));
                    break;
                }
                default:
                    break;
            }
        }
    }
}