
#include <string>
#include <vector>

#include "tasks/reception_task.h"
#include "tasks/initialize_server_task.h"
#include "tasks/receive_requests_task.h"
#include "tasks/prune_connections_task.h"
#include "state.h"

#define PORT 8080

int main() {
    State* state = new State;

    InitializeServerTask(state).perform();

    state->scheduler->add(new ReceptionTask(state));
    state->scheduler->add(new ReceiveRequestsTask(state));
    state->scheduler->add(new PruneConnectionsTask(state));

    state->scheduler->run();

    return 0;
}
