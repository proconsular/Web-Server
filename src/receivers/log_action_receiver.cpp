//
// Created by Chris Luttio on 4/5/21.
//

#include "log_action_receiver.h"
#include "socket.h"
#include "utils.h"
#include "configuration.h"
#include "http_request_envelope.h"
#include "http_response_envelope.h"
#include "client_request.h"

#include <iostream>

LogActionReceiver::LogActionReceiver(std::string filename): _filename(std::move(filename)) {
    _file = fopen("log.txt", "wb");
}

void LogActionReceiver::receive(const Action &action) {
    std::string output;
    switch (action.type) {
        case SetConfiguration: {
            auto config = std::static_pointer_cast<Configuration>(action.data);
            output.append(string_format("LOADED: config: %s; path: %s, port: %i", config->from_file.c_str(), config->base_url.to_string().c_str(), config->port));
            break;
        }
        case CreateServerSocket: {
            auto socket = std::static_pointer_cast<Socket>(action.data);
            output.append(string_format("STARTED: server: %i", socket->port()));
            break;
        }
        case CreateHttpRequest: {
            auto envelope = std::static_pointer_cast<HTTPRequestEnvelope>(action.data);
            output.append(
                    string_format("REQUEST: %s %s %s",
                                  envelope->request->method.c_str(),
                                  envelope->request->uri.to_string().c_str(),
                                  envelope->request->version.c_str())
                                  );
            break;
        }
        case CreateClientConnection: {
            auto connection = std::static_pointer_cast<Connection>(action.data);
            output.append(string_format("ESTABLISHED: client connection: %s", connection->id().c_str()));
            break;
        }
        case RemoveClientConnection: {
            auto connection = std::static_pointer_cast<Connection>(action.data);
            output.append(string_format("DISCONNECTED: client connection: %s", connection->id().c_str()));
            break;
        }
        case CreateClientRequest: {
            auto request = std::static_pointer_cast<ClientRequest>(action.data);
            std::map<Requests, std::string> table = {
                    {Unsupported, "Unsupported"},
                    {RetrieveFile, "Retrieve File"}
            };
            output.append(string_format("CREATED: request: %s: %s", request->id().c_str(), table[request->type].c_str()));
            break;
        }
        case ModifyClientRequest: {
            auto request = std::static_pointer_cast<ClientRequest>(action.data);
            std::map<Requests, std::string> table = {
                    {Unsupported, "Unsupported"},
                    {RetrieveFile, "Retrieve File"}
            };
            std::map<RequestStatus, std::string> status = {
                    {New, "New"},
                    {Working, "Working"},
                    {Complete, "Complete"},
                    {Failed, "Failed"},
            };
            output.append(string_format("UPDATED: request: %s: %s, %s", request->id().c_str(), table[request->type].c_str(), status[request->status].c_str()));
            break;
        }
        case CreateHttpResponse: {
            auto response = std::static_pointer_cast<HTTPResponseEnvelope>(action.data);
            output.append(string_format("RESPONSE: %i %s %s to %s", response->response->code, response->response->status.c_str(), response->response->version.c_str(), response->connection->id().c_str()));
            break;
        }
        default:
            break;
    }
    if (output.empty()) {
        return;
    }
    auto timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto tm = std::ctime(&timenow);
    tm[strlen(tm) - 1] = '\0';
    output.insert(0, string_format("%s:  ", tm));
    output.append("\n");
    fwrite(output.c_str(), sizeof(char), output.size(), _file);
    fflush(_file);
    fwrite(output.c_str(), sizeof(char), output.size(), stdout);
}