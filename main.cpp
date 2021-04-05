
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

#include "state.h"

int main() {
    auto* state = new State;

    state->scheduler->add(new LoadConfigurationTask(state));
    state->scheduler->add(new InitializeServerTask(state));
    state->scheduler->add(new ReceptionTask(state));
    state->scheduler->add(new PruneConnectionsTask(state));
    state->scheduler->add(new ReceiveRequestsTask(state));
    state->scheduler->add(new ProcessHTTPRequestsTask(state));
    state->scheduler->add(new InitializeClientRequestsTask(state));
    state->scheduler->add(new FinalizeClientRequestsTask(state));
    state->scheduler->add(new SendResponsesTask(state));
    state->scheduler->add(new InitializeHTTPRequestConnectionsTask(state));
    state->scheduler->add(new SendHTTPRequestsTask(state));
    state->scheduler->add(new ReceiveHTTPResponsesTask(state));
    state->scheduler->add(new LogEventsTask(state, "log.txt"));

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
