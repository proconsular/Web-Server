//
// Created by Chris Luttio on 3/27/21.
//

#include "initialize_client_requests_task.h"
#include "load_requested_file_task.h"

void InitializeClientRequestsTask::perform() {
    for (const auto& pair: state->requests) {
        auto request = std::make_shared<ClientRequest>(*pair.second);
        if (request->status == RequestStatus::New) {
            switch (request->type) {
                case RetrieveFile: {
                    state->scheduler->add(std::make_shared<LoadRequestedFileTask>(_controller, request, state->config));
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