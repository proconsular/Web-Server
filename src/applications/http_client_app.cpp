//
// Created by Chris Luttio on 4/15/21.
//

#include "http_client_app.h"
#include "state.h"

#include "controllers/direct_controller.h"
#include "actions/action.h"

#include "tasks/initialize_http_request_connections_task.h"
#include "tasks/send_http_requests_task.h"
#include "tasks/receive_http_responses_task.h"
#include "tasks/watch_http_response_task.h"

#include "receivers/log_action_receiver.h"

bool HttpClientApp::send(const std::string &domain, const std::shared_ptr<HTTPRequest> &request, std::shared_ptr<HTTPResponse>& out) {
    auto state = std::make_shared<State>();
    auto controller = std::make_shared<DirectController>(state);

    if (!_log_filename.empty())
        controller->add_receiver(std::make_shared<LogActionReceiver>(_log_filename));

    controller->apply(Action(StartApp, std::make_shared<std::string>("Http Client")));

    auto url = URL::parse(domain);
    request->headers["Host"] = url.domain_to_cstr();
    controller->apply(Action(CreateOutboundHttpRequest, std::make_shared<HTTPRequestCarrier>(url, request)));
    auto id = state->outbound_http_request_queue.begin()->second->id();

    std::shared_ptr<HTTPResponse> resp;

    auto trigger = [&resp, &state](const std::shared_ptr<HTTPResponse>& response) {
        resp = response;
        state->scheduler->terminate();
    };

    state->scheduler->add(std::make_shared<InitializeHTTPRequestConnectionsTask>(state, controller));
    state->scheduler->add(std::make_shared<SendHTTPRequestsTask>(state, controller));
    state->scheduler->add(std::make_shared<ReceiveHTTPResponsesTask>(state, controller));
    state->scheduler->add(std::make_shared<WatchHttpResponseTask>(state, id, trigger));

    state->scheduler->run();

    if (resp != nullptr) {
        out = resp;
    }

    return resp != nullptr;
}