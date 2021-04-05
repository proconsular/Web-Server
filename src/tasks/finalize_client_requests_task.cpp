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
                        response->headers["Content-Type"] = getContentType(request->path.extension);
                        break;
                    }
                    default:
                        response->status = "Not Implemented";
                        response->code = 501;
                        break;
                }
                state->outbound_http_response_queue.emplace_back(request->connection, response);
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
                state->outbound_http_response_queue.emplace_back(request->connection, response);
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
        } else {
            i++;
        }
    }
}

std::string FinalizeClientRequestsTask::getContentType(const std::string &ext) {
    std::map<std::string, std::string> ext_table = {
            {"html", "text/html"},
            {"js", "text/javascript"},
            {"jpg", "image/jpeg"},
            {"jpeg", "image/jpeg"},
            {"png", "image/png"},
            {"json", "application/json"},
            {"ttf", "font/ttf"},
            {"txt", "text/plain"},
            {"wav", "audio/wav"},
            {"xml", "application/xml"},
            {"bin", "application/octet-stream"},
            {"css", "text/css"},
    };
    if (ext_table.find(ext) != ext_table.end()) {
        return ext_table[ext];
    }
    return "text/plain";
}