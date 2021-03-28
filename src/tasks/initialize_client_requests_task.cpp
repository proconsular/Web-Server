//
// Created by Chris Luttio on 3/27/21.
//

#include "initialize_client_requests_task.h"
#include "load_requested_file_task.h"

void InitializeClientRequestsTask::perform() {
    for (auto request: state->requests) {
        if (request->status == RequestStatus::New) {
            switch (request->type) {
                case RetrieveFile: {
                    state->scheduler->add(new LoadRequestedFileTask(request, state->config));
                    request->status = RequestStatus::Working;
                    std::cout << "INFO: Retrieve File Request WORKING" << std::endl;
                    break;
                }
                default:
                    break;
            }
        }
    }
}