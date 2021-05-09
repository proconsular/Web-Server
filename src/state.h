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
#include "objects/route.h"
#include "objects/access_profile.h"
#include "objects/user_account.h"

class State {
public:
    State() {
        scheduler = std::make_shared<TaskScheduler>();
        config = std::make_shared<Configuration>();
        ssl_enabled = false;

        AccessProfile public_access("public");
        public_access.allowed_methods = {"GET"};
        public_access.request_size_limit = 10 * 1024;

        access_level = "public";
        access_profiles["public"] = public_access;

        accounts.push_back(std::make_shared<UserAccount>("admin", "hello", "admin"));
    }

    const SSL_METHOD *ssl_method;
    SSL_CTX *ssl_context;

    Socket server_socket;
    Socket tls_socket;

    std::map<std::string, std::shared_ptr<Connection>> connections;
    std::map<std::string, std::shared_ptr<ClientRequest>> requests;

    std::map<std::string, std::shared_ptr<HTTPRequestCarrier>> active_requests;

    std::vector<HTTPRequestEnvelope> inbound_http_request_queue;
    std::vector<HTTPResponseEnvelope> outbound_http_response_queue;

    std::shared_ptr<TaskScheduler> scheduler;

    std::shared_ptr<Configuration> config;
    std::vector<Route> routes;
    std::map<std::string, AccessProfile> access_profiles;

    std::vector<std::shared_ptr<UserAccount>> accounts;

    std::string access_level;

    bool ssl_enabled;

    AccessProfile get_access_profile() {
        return access_profiles[access_level];
    }
};


#endif //P8_WEB_SERVER_STATE_H
