//
// Created by Chris Luttio on 3/27/21.
//

#include "finalize_client_requests_task.h"
#include "client_request.h"

#include <memory>

void FinalizeClientRequestsTask::perform() {
    for (const auto& pair: state->requests) {
        auto request = std::make_shared<ClientRequest>(*pair.second);
        switch (request->status) {
            case Complete: {
                auto response = std::make_shared<HTTPResponse>(200, "OK", "HTTP/1.1");
                switch (request->type) {
                    case RetrieveFile: {
                        response->body = request->data;
                        response->headers["Content-Type"] = std::make_shared<std::string>(getContentType(request->path.extension));
                        break;
                    }
                    default:
                        response->status = "Not Implemented";
                        response->code = 501;
                        break;
                }
                _controller->apply(Action(CreateHttpResponse, std::make_shared<HTTPResponseEnvelope>(request->connection, response)));
                break;
            }
            case Failed: {
                auto response = std::make_shared<HTTPResponse>(500, "Internal Server Error", "HTTP/1.1");
                switch (request->type) {
                    case RetrieveFile: {
                        response->code = 404;
                        response->status = "Not Found";
                        break;
                    }
                    default:
                        break;
                }
                _controller->apply(Action(CreateHttpResponse, std::make_shared<HTTPResponseEnvelope>(request->connection, response)));
                break;
            }
            case New: {
                if (request->type == Unsupported) {
                    auto response = std::make_shared<HTTPResponse>(501, "Not Implemented", "HTTP/1.1");
                    _controller->apply(Action(CreateHttpResponse, std::make_shared<HTTPResponseEnvelope>(request->connection, response)));
                }
                break;
            }
            default:
                break;
        }
    }
    for (auto iter = state->requests.cbegin(), next_iter = iter; iter != state->requests.cend(); iter = next_iter) {
        auto request = iter->second;
        next_iter++;
        if (request->status == Complete || request->status == Failed || (request->status == New && request->type == Unsupported)) {
            _controller->apply(Action(RemoveClientRequest, request));
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