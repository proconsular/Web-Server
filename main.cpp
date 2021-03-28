
#include <string>
#include <vector>

#include "tasks/reception_task.h"
#include "tasks/initialize_server_task.h"
#include "tasks/receive_requests_task.h"
#include "tasks/prune_connections_task.h"
#include "tasks/process_http_requests_task.h"
#include "tasks/initialize_client_requests_task.h"
#include "tasks/send_responses_task.h"
#include "tasks/finalize_client_requests_task.h"

#include "state.h"

#define PORT 8080

int main() {
    State* state = new State;

    state->config.base_url = "web/";

    InitializeServerTask(state).perform();

    state->scheduler->add(new ReceptionTask(state));
    state->scheduler->add(new PruneConnectionsTask(state));
    state->scheduler->add(new ReceiveRequestsTask(state));
    state->scheduler->add(new ProcessHTTPRequestsTask(state));
    state->scheduler->add(new InitializeClientRequestsTask(state));
    state->scheduler->add(new FinalizeClientRequestsTask(state));
    state->scheduler->add(new SendResponsesTask(state));

    state->scheduler->run();

    return 0;
}
