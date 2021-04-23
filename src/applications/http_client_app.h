//
// Created by Chris Luttio on 4/15/21.
//

#ifndef P8_WEB_SERVER_HTTP_CLIENT_APP_H
#define P8_WEB_SERVER_HTTP_CLIENT_APP_H

#include <utility>
#include <memory>

#include "state.h"

#include "controllers/direct_controller.h"
#include "actions/action.h"

#include "tasks/initialize_http_request_connections_task.h"
#include "tasks/send_http_requests_task.h"
#include "tasks/receive_http_responses_task.h"
#include "tasks/watch_http_response_task.h"

#include "receivers/log_action_receiver.h"

class HttpClientApp {
public:
    explicit HttpClientApp();

    bool send(const std::string&, const std::shared_ptr<HttpMessage>&, std::shared_ptr<HttpMessage>&);

    std::shared_ptr<State> state() const {
        return _state;
    }
private:
    std::string _log_filename;

    std::shared_ptr<State> _state;
    std::shared_ptr<Controller> _controller;
};


#endif //P8_WEB_SERVER_HTTP_CLIENT_APP_H
