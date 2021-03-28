//
// Created by Chris Luttio on 3/25/21.
//

#include "prune_connections_task.h"
#include <iostream>
#include <chrono>

void PruneConnectionsTask::perform() {
    for (int i = state->connections.size() - 1; i >= 0; i--) {
        auto conn = state->connections[i];
        auto time_since_read = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - conn->last_read);
        if (!conn->alive || (time_since_read.count() >= 30 && conn->active_requests == 0)) {
            state->connections.erase(state->connections.begin() + i);
            delete conn;
            std::cout << "INFO: Removed Connection " << i << std::endl;
        }
    }
}