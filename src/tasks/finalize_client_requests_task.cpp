//
// Created by Chris Luttio on 3/27/21.
//

#include "finalize_client_requests_task.h"
#include "client_request.h"
#include "objects/http_message.h"

#include <memory>

void FinalizeClientRequestsTask::perform() {
    for (const auto& pair: state->requests) {
        auto request = std::make_shared<ClientRequest>(*pair.second);
        switch (request->status) {
            case Complete: {
                auto response = std::make_shared<HttpMessage>(RESPONSE);
                response->code = 200;
                response->version = "HTTP/1.1";
                response->status = "OK";
                switch (request->type) {
                    case RetrieveFile: {
                        response->body = request->data;
                        response->headers["Content-Type"] = std::make_shared<std::string>(getContentType(request->path.extension));
                        for (const auto & response_header : request->response_headers) {
                            response->headers[response_header.first] = std::make_shared<std::string>(response_header.second);
                        }
                        break;
                    }
                    case BadRequest: {
                        response->body = std::make_shared<std::string>("");
                        response->code = 400;
                        response->status = "Bad Request";
                        response->headers["Content-Length"] = std::make_shared<std::string>("0");
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
                auto response = std::make_shared<HttpMessage>(RESPONSE);
                response->code = 500;
                response->version = "HTTP/1.1";
                response->status = "Internal Server Error";
                switch (request->type) {
                    case RetrieveFile: {
                        response->code = 404;
                        response->status = "Not Found";
                        response->body = std::make_shared<std::string>("");
                        response->headers["Content-Length"] = std::make_shared<std::string>("0");
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
                    auto response = std::make_shared<HttpMessage>(RESPONSE);
                    response->code = 501;
                    response->version = "HTTP/1.1";
                    response->status = "Not Implemented";
                    response->headers["Content-Length"] = std::make_shared<std::string>("0");
                    response->body = std::make_shared<std::string>("");
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