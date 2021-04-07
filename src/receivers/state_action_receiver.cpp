//
// Created by Chris Luttio on 4/5/21.
//

#include "state_action_receiver.h"

void StateActionReceiver::receive(const Action &action) {
    switch (action.type) {
        case SetConfiguration: {
            auto* config = reinterpret_cast<Configuration*>(action.data);
            _state->config = *config;
            break;
        }
        case CreateServerSocket: {
            auto* socket = reinterpret_cast<Socket*>(action.data);
            _state->server_socket = *socket;
            break;
        }
        case CreateHttpRequest: {
            auto* envelope = reinterpret_cast<HTTPRequestEnvelope*>(action.data);
            _state->inbound_http_request_queue.push_back(*envelope);
            break;
        }
        case CreateClientConnection:
        case ModifyClientConnection: {
            auto* connection = reinterpret_cast<Connection*>(action.data);
            _state->connections[connection->id()] = connection;
            break;
        }
        case RemoveClientConnection: {
            auto* connection = reinterpret_cast<Connection*>(action.data);
            _state->connections.erase(connection->id());
            close(connection->socket.id());
            break;
        }
        case CreateClientRequest:
        case ModifyClientRequest: {
            auto* request = reinterpret_cast<ClientRequest*>(action.data);
            _state->requests[request->id()] = request;
            break;
        }
        case RemoveClientRequest: {
            auto* request = reinterpret_cast<ClientRequest*>(action.data);
            _state->requests.erase(request->id());
            break;
        }
        case CreateHttpResponse: {
            auto* response = reinterpret_cast<HTTPResponseEnvelope*>(action.data);
            _state->outbound_http_response_queue.push_back(*response);
            break;
        }
        case ClearHttpResponses: {
            for (auto response: _state->outbound_http_response_queue) {
                response.connection->active_requests--;
            }
            _state->outbound_http_response_queue.clear();
            break;
        }
        default:
            break;
    }
}