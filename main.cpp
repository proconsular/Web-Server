
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
#include "tasks/load_routing_data_task.h"
#include "tasks/tls_reception_task.h"
#include "tasks/initialize_tls_server_socket_task.h"

#include "controllers/direct_controller.h"
#include "receivers/log_action_receiver.h"

#include "state.h"

int main() {
    auto state = std::make_shared<State>();
    auto controller = std::make_shared<DirectController>(state);
    controller->add_receiver(std::make_shared<LogActionReceiver>("log.txt"));

    controller->apply(Action(StartApp, std::make_shared<std::string>("Http Server")));

    state->scheduler->add(std::make_shared<LoadConfigurationTask>(controller));
    state->scheduler->add(std::make_shared<LoadRoutingDataTask>(state));
    state->scheduler->add(std::make_shared<InitializeServerTask>(state, controller));
    state->scheduler->add(std::make_shared<InitializeTLSServerSocketTask>(state, controller));
    state->scheduler->add(std::make_shared<ReceptionTask>(state, controller));
    state->scheduler->add(std::make_shared<TLSReceptionTask>(state, controller));
    state->scheduler->add(std::make_shared<PruneConnectionsTask>(state, controller));
    state->scheduler->add(std::make_shared<ReceiveRequestsTask>(state, controller));
    state->scheduler->add(std::make_shared<ProcessHTTPRequestsTask>(state, controller));
    state->scheduler->add(std::make_shared<InitializeClientRequestsTask>(state, controller));
    state->scheduler->add(std::make_shared<FinalizeClientRequestsTask>(state, controller));
    state->scheduler->add(std::make_shared<SendResponsesTask>(state, controller));
    state->scheduler->add(std::make_shared<InitializeHTTPRequestConnectionsTask>(state, controller));
    state->scheduler->add(std::make_shared<SendHTTPRequestsTask>(state, controller));
    state->scheduler->add(std::make_shared<ReceiveHTTPResponsesTask>(state, controller));

    state->scheduler->run();

    return 0;
}
