//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_STATE_H
#define P8_WEB_SERVER_STATE_H

#include "task_scheduler.h"
#include "connection.h"
#include "client_request.h"
#include "http_request_envelope.h"
#include "http_response_envelope.h"
#include "configuration.h"
#include "http_request_carrier.h"

#include <vector>

class State {
public:
    State() {
        scheduler = std::make_shared<TaskScheduler>();
    }

    Socket server_socket;
    std::map<std::string, std::shared_ptr<Connection>> connections;
    std::map<std::string, std::shared_ptr<ClientRequest>> requests;

    std::map<std::string, std::shared_ptr<HTTPRequestCarrier>> outbound_http_request_queue;
//    std::vector<HTTPResponseEnvelope> inbound_http_response_queue;

    std::vector<HTTPRequestEnvelope> inbound_http_request_queue;
    std::vector<HTTPResponseEnvelope> outbound_http_response_queue;

    std::shared_ptr<TaskScheduler> scheduler;

    std::shared_ptr<Configuration> config;
};


#endif //P8_WEB_SERVER_STATE_H
