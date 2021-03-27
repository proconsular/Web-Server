//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_STATE_H
#define P8_WEB_SERVER_STATE_H

#include "task_scheduler.h"
#include "connection.h"
#include "client_request.h"

#include <vector>

class State {
public:
    State() {
        scheduler = new TaskScheduler;
    }

    Socket server_socket;
    std::vector<Connection*> connections;
    std::vector<ClientRequest*> requests;

    TaskScheduler* scheduler;
};


#endif //P8_WEB_SERVER_STATE_H
