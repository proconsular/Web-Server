//
// Created by Chris Luttio on 4/4/21.
//

#ifndef P8_WEB_SERVER_ADD_CLIENT_CONNECTION_ACTION_H
#define P8_WEB_SERVER_ADD_CLIENT_CONNECTION_ACTION_H

#include "action.h"
#include "connection.h"

class AddClientConnectionAction: public Action {
public:
    explicit AddClientConnectionAction(Connection* connection): Action(CreateClientConnection), connection(connection) {}

    Connection* connection;
};

#endif //P8_WEB_SERVER_ADD_CLIENT_CONNECTION_ACTION_H
