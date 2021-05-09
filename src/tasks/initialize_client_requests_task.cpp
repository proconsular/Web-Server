//
// Created by Chris Luttio on 3/27/21.
//

#include "initialize_client_requests_task.h"
#include "load_requested_file_task.h"
#include "utils.h"
#include "route_resolver.h"
#include "tasks/authorize_user_task.h"
#include "jwt-cpp/jwt.h"
#include "json.hpp"

using json = nlohmann::json;

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

                    RouteResolver resolver;
                    resolver.resolve(state->routes, request->uri.to_string());

                    bool authorized = false;

                    if (resolver.attributes.find("Access") != resolver.attributes.end()) {
                        std::string role = resolver.attributes["Access"];
                        if (role != "public") {
                            if (headers.find("Authorization") != headers.end()) {
                                auto authorization = headers["Authorization"];
                                auto components = split_string(" ", *authorization);
                                if (components.size() == 2 && components[0] == "Bearer") {
                                    std::string access_token = components[1];

                                    for (const auto& account : state->accounts) {
                                        if (account->role == role) {
                                            for (const auto& auth : account->authorizations) {
                                                if (auth.access_token == access_token) {
                                                    auto decoded = jwt::decode(access_token);

                                                    auto verifier = jwt::verify()
                                                            .allow_algorithm(jwt::algorithm::hs256{"helloworld"})
                                                            .with_issuer("auth0");

                                                    authorized = true;

                                                    try {
                                                        verifier.verify(decoded);
                                                    } catch (std::exception& e) {
                                                        authorized = false;
                                                    }

                                                    break;
                                                }
                                            }
                                            if (authorized)
                                                break;
                                        }
                                    }
                                }
                            }
                        } else {
                            authorized = true;
                        }
                    } else {
                        authorized = true;
                    }

                    if (authorized) {
                        request->response_headers = resolver.attributes;
                        request->uri = URL::parse(resolver.url);
                        request->type = RetrieveFile;
                    } else {
                        request->type = Forbidden;
                        request->status = Failed;
                    }

                    _controller->apply(Action(ModifyClientRequest, request));
                    break;
                }
                case Authorize: {
                    state->scheduler->add(std::make_shared<AuthorizeUserTask>(state, _controller, request));
                    request->status = Working;
                    _controller->apply(Action(ModifyClientRequest, request));
                    break;
                }
                case Refresh: {

                    bool authorized = false;

                    std::string new_token;

                    if (!request->data->empty()) {
                        json body = json::parse(*request->data);
                        std::string refresh_token;
                        if (body.find("refresh_token") != body.end())
                            refresh_token = body["refresh_token"];

                        for (const auto& account: state->accounts) {
                            for (auto& auth: account->authorizations) {
                                if (auth.refresh_token == refresh_token) {
                                    auto decoded = jwt::decode(refresh_token);

                                    auto verifier = jwt::verify()
                                            .allow_algorithm(jwt::algorithm::hs256{"helloworld"})
                                            .with_issuer("auth0");

                                    authorized = true;

                                    try {
                                        verifier.verify(decoded);
                                    } catch (std::exception& e) {
                                        authorized = false;
                                    }

                                    if (authorized) {
                                        auto now = std::chrono::system_clock::now();
                                        auth.access_token = jwt::create()
                                                .set_issuer("auth0")
                                                .set_type("JWT")
                                                .set_issued_at(now)
                                                .set_expires_at(now + std::chrono::minutes{60})
                                                .set_payload_claim("username", jwt::claim(std::string(account->username)))
                                                .set_payload_claim("password", jwt::claim(std::string(account->password)))
                                                .set_payload_claim("client_id", jwt::claim(auth.client_id))
                                                .sign(jwt::algorithm::hs256{"helloworld"});
                                        new_token = auth.access_token;
                                    }

                                    break;
                                }
                            }
                            if (authorized)
                                break;
                        }
                    }

                    if (authorized) {
                        json body({{"access_token", new_token}});
                        request->data = std::make_shared<std::string>(body.dump());
                        request->status = Complete;
                    } else {
                        request->type = Authorize;
                        request->status = Failed;
                    }

                    _controller->apply(Action(ModifyClientRequest, request));
                    break;
                }
                default:
                    break;
            }
        }
    }
}