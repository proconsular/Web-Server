//
// Created by Chris Luttio on 3/27/21.
//

#include "finalize_client_requests_task.h"

void FinalizeClientRequestsTask::perform() {
    for (auto request: state->requests) {
        switch (request->status) {
            case Complete: {
                auto* response = new HTTPResponse(200, "OK", "HTTP/1.1");
                switch (request->type) {
                    case RetrieveFile: {
                        response->body = request->data;
                        break;
                    }
                    default:
                        response->status = "Not Implemented";
                        response->code = 501;
                        break;
                }
                state->http_response_queue.emplace_back(request->connection, response);
                break;
            }
            case Failed: {
                auto* response = new HTTPResponse(500, "Internal Server Error", "HTTP/1.1");
                switch (request->type) {
                    case RetrieveFile: {
                        response->code = 404;
                        response->status = "Not Found";
                        break;
                    }
                    default:
                        break;
                }
                state->http_response_queue.emplace_back(request->connection, response);
                break;
            }
            default:
                break;
        }
    }
    for (auto i = state->requests.begin(); i != state->requests.end();) {
        auto status = (*i)->status;
        if (status == RequestStatus::Complete || status == RequestStatus::Failed) {
            state->requests.erase(i);
            std::cout << "INFO: Request Finalized" << std::endl;
        } else {
            i++;
        }
    }
}