//
// Created by Chris Luttio on 4/4/21.
//

#include "direct_controller.h"
#include "actions/new_http_request_action.h"
#include "actions/create_server_socket_action.h"
#include "actions/load_configuration_action.h"
#include "actions/add_client_connection_action.h"
#include "actions/add_client_request_action.h"
#include "actions/modify_client_request_action.h"

void DirectController::apply(Action *action) {
    switch (action->type) {
        case SetConfiguration: {
            auto* _action = reinterpret_cast<LoadConfigurationAction*>(action);
            _state->config = _action->config;
            break;
        }
        case CreateServerSocket: {
            auto* server_action = reinterpret_cast<CreateServerSocketAction*>(action);
            _state->server_socket = server_action->socket;
            break;
        }
        case CreateHttpRequest: {
            auto* http_action = reinterpret_cast<NewHttpRequestAction*>(action);
            _state->inbound_http_request_queue.push_back(http_action->http_request_envelope);
            break;
        }
        case CreateClientConnection: {
            auto* _action = reinterpret_cast<AddClientConnectionAction*>(action);
            _state->connections.push_back(_action->connection);
            break;
        }
        case CreateClientRequest: {
            auto* _action = reinterpret_cast<AddClientRequestAction*>(action);
            _state->requests.push_back(_action->request);
            break;
        }
        case ModifyClientRequest: {
            auto* _action = reinterpret_cast<ModifyClientRequestAction*>(action);
//            _state->requests.push_back(_action->request);
            break;
        }
        default:
            break;
    }
}