//
// Created by Chris Luttio on 3/25/21.
//

#include "reception_task.h"

#include <arpa/inet.h>
#include <iostream>

void ReceptionTask::perform() {
    Socket new_client;
    if (state->server_socket.accept(new_client)) {
        _controller->apply(Action(CreateClientConnection, std::make_shared<Connection>(new_client)));
    }
}

