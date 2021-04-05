//
// Created by Chris Luttio on 4/4/21.
//

#ifndef P8_WEB_SERVER_CREATE_SERVER_SOCKET_ACTION_H
#define P8_WEB_SERVER_CREATE_SERVER_SOCKET_ACTION_H

#include "action.h"
#include "socket.h"

class CreateServerSocketAction: public Action {
public:
    explicit CreateServerSocketAction(Socket socket): Action(CreateServerSocket), socket(socket) {}

    Socket socket;
};

#endif //P8_WEB_SERVER_CREATE_SERVER_SOCKET_ACTION_H
