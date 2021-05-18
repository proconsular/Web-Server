//
// Created by Chris Luttio on 3/27/21.
//

#include "process_http_requests_task.h"
#include "json.hpp"

#include <algorithm>

using json = nlohmann::json;

void ProcessHTTPRequestsTask::perform() {
    for (const auto& envelope: state->inbound_http_request_queue) {
        auto request = envelope.request;
        auto client_request = std::make_shared<ClientRequest>();
        client_request->connection = envelope.connection;

        auto connection_header = request->headers.find("Connection");
        if (connection_header != request->headers.end()) {
            std::string value(*connection_header->second);
            std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return std::tolower(c); });
            if (value == "close") {
                envelope.connection->persistence = Connection::CLOSE;
            }
        }

        auto url = request->url.to_string();

        if (request->method == "GET") {
            bool websocket = false;
            if (url == "/") {
                if (request->headers.find("Connection") != request->headers.end() && request->headers.find("Upgrade") != request->headers.end()) {
                    if (*request->headers["Upgrade"] == "websocket") {
                        auto key = request->headers["Sec-WebSocket-Key"];
                        auto version = request->headers["Sec-WebSocket-Version"];
                        if (key != nullptr && version != nullptr) {
                            std::string accept = *key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

                            uint8_t buffer[20];
                            SHA1(reinterpret_cast<const uint8_t *>(accept.c_str()), accept.size(), buffer);
                            std::string encoded = encode_base_64(buffer, 20);

                            envelope.connection->protocol = WebSocket;
                            envelope.connection->websocket_state = CONNECTING;
                            client_request->type = WebSocketUpgrade;
                            client_request->response_headers["Sec-WebSocket-Accept"] = encoded;
                            client_request->status = Complete;
                            websocket = true;
                        }
                    }
                }
            }
            if (!websocket) {
                if (state->routes.empty()) {
                    client_request->type = Requests::RetrieveFile;
                    client_request->uri = request->url;
                } else {
                    client_request->type = ResolveRoute;
                    client_request->uri = request->url;
                    client_request->http_request = request;
                }
            }
        }

        if (request->method == "POST") {
            if (url == "/authorize") {
                client_request->type = Authorize;
                client_request->data = request->body;
            } else if (url == "/authorize/refresh") {
                client_request->type = Refresh;
                client_request->data = request->body;
            } else if (url == "/track") {
                auto data = json::parse(*request->body);
                auto name = data["name"];
                auto session = std::make_shared<Session>(name, client_request->connection->socket.ip());
                state->sessions[session->id()] = session;
                client_request->data = std::make_shared<std::string>(json({{"id", session->id()}}).dump());
                client_request->type = Created;
                client_request->status = Complete;
            } else if (url.substr(0, 7) == "/track/") {
                auto session_id = url.substr(7);
                if (state->sessions.find(session_id) != state->sessions.end()) {
                    auto session = state->sessions[session_id];

                    json payload(*request->body);

                    SessionRecord record;
                    record.timestamp = std::chrono::high_resolution_clock::now();
                    if (payload.find("scroll_height") != payload.end())
                        record.scroll_height = payload["scroll_height"];
                    if (payload.find("window_height") != payload.end())
                        record.window_height = payload["window_height"];
                    if (payload.find("document_height") != payload.end())
                        record.document_height = payload["document_height"];

                    session->records.push_back(record);

                    client_request->type = Created;
                    client_request->status = Complete;
                }
            }
        }

        if (client_request->type == BadRequest) {
            client_request->connection->persistence = Connection::CLOSE;
            client_request->status = Complete;
        }

        _controller->apply(Action(CreateClientRequest, client_request));
    }
    _controller->apply(Action(ClearHttpRequests));
}