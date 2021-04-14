
#include <string>

#include "tasks/reception_task.h"
#include "tasks/initialize_server_task.h"
#include "tasks/receive_requests_task.h"
#include "tasks/prune_connections_task.h"
#include "tasks/process_http_requests_task.h"
#include "tasks/initialize_client_requests_task.h"
#include "tasks/send_responses_task.h"
#include "tasks/finalize_client_requests_task.h"
#include "tasks/load_configuration_task.h"
#include "tasks/initialize_http_request_connections_task.h"
#include "tasks/receive_http_responses_task.h"
#include "tasks/send_http_requests_task.h"

#include "controllers/direct_controller.h"

#include "state.h"
#include "utils.h"

int main() {
    auto state = std::make_shared<State>();
    auto controller = std::make_shared<DirectController>(state);

    state->scheduler->add(std::make_shared<LoadConfigurationTask>(controller));
    state->scheduler->add(std::make_shared<InitializeServerTask>(state, controller));
    state->scheduler->add(std::make_shared<ReceptionTask>(state, controller));
    state->scheduler->add(std::make_shared<PruneConnectionsTask>(state, controller));
    state->scheduler->add(std::make_shared<ReceiveRequestsTask>(state, controller));
    state->scheduler->add(std::make_shared<ProcessHTTPRequestsTask>(state, controller));
    state->scheduler->add(std::make_shared<InitializeClientRequestsTask>(state, controller));
    state->scheduler->add(std::make_shared<FinalizeClientRequestsTask>(state, controller));
    state->scheduler->add(std::make_shared<SendResponsesTask>(state, controller));
    state->scheduler->add(std::make_shared<InitializeHTTPRequestConnectionsTask>(state, controller));
    state->scheduler->add(std::make_shared<SendHTTPRequestsTask>(state, controller));
    state->scheduler->add(std::make_shared<ReceiveHTTPResponsesTask>(state, controller));

    controller->apply(Action(StartProgram));

    auto url = URL::parse("http://voxday.blogspot.com/");

    auto request = std::make_shared<HTTPRequest>();
    request->method = "GET";
    request->version = "HTTP/1.1";
    request->uri = URL::parse("/");
    request->headers["Content-Length"] = "0";
    request->headers["Host"] = url.domain_to_cstr();

    controller->apply(Action(CreateOutboundHttpRequest, std::make_shared<HTTPRequestCarrier>(url, request)));

    state->scheduler->run();

    return 0;
}
