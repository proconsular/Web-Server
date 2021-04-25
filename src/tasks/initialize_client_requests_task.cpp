//
// Created by Chris Luttio on 3/27/21.
//

#include "initialize_client_requests_task.h"
#include "load_requested_file_task.h"
#include "utils.h"

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
                case ResolveRoute: {
                    auto headers = request->http_request->headers;
                    auto route = request->route;
                    std::string key;
                    if (headers.find("Accept") != headers.end()) {
                        auto header = headers["Accept"];
                        auto list = parse_accept_header(*header);

                        for (const auto& item : list) {
                            if (route.content_type.find(item) != route.content_type.end()) {
                                key = item;
                                break;
                            }
                        }
                        request->uri = URL::parse(route.content_type[key]);
                    }
                    if (key.empty()) {
                        for (auto iter = route.content_type.cbegin(); iter != route.content_type.cend(); iter++) {
                            if (iter->second == route.default_file) {
                                key = iter->first;
                                break;
                            }
                        }
                        if (!key.empty()) {
                            request->response_headers["Content-Type"] = key;
                        }
                        request->uri = URL::parse(route.default_file);
                    }
                    if (route.language.size() == 1) {
                        request->response_headers["Content-Language"] = route.language.begin()->first;
                    }
                    if (!route.cache_control.empty()) {
                        request->response_headers["Cache-Control"] = route.cache_control;
                    }

                    request->type = RetrieveFile;

                    _controller->apply(Action(ModifyClientRequest, request));
                    break;
                }
                default:
                    break;
            }
        }
    }
}