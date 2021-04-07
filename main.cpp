
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
#include "tasks/log_events_task.h"

#include "controllers/direct_controller.h"

#include "state.h"
#include "utils.h"

int main() {
    auto* state = new State;
    auto* controller = new DirectController(state);

    state->scheduler->add(new LoadConfigurationTask(controller));
    state->scheduler->add(new InitializeServerTask(state, controller));
    state->scheduler->add(new ReceptionTask(state, controller));
    state->scheduler->add(new PruneConnectionsTask(state, controller));
    state->scheduler->add(new ReceiveRequestsTask(state, controller));
    state->scheduler->add(new ProcessHTTPRequestsTask(state, controller));
    state->scheduler->add(new InitializeClientRequestsTask(state, controller));
    state->scheduler->add(new FinalizeClientRequestsTask(state, controller));
    state->scheduler->add(new SendResponsesTask(state, controller));
    state->scheduler->add(new InitializeHTTPRequestConnectionsTask(state));
    state->scheduler->add(new SendHTTPRequestsTask(state));
    state->scheduler->add(new ReceiveHTTPResponsesTask(state));

    auto url = URL::parse("http://www.google.com");

    auto* request = new HTTPRequest;
    request->method = "GET";
    request->version = "HTTP/1.1";
    request->uri = URL::parse("/");
    request->headers["Content-Length"] = "0";
    request->headers["Host"] = url.domain_to_cstr();

//    state->outbound_http_request_queue.push_back(new HTTPRequestCarrier(url, request));

    state->scheduler->run();

    return 0;
}
