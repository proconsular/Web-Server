//
// Created by Chris Luttio on 4/5/21.
//

#ifndef P8_WEB_SERVER_ADD_CLIENT_REQUEST_ACTION_H
#define P8_WEB_SERVER_ADD_CLIENT_REQUEST_ACTION_H

#include "action.h"
#include "client_request.h"

class AddClientRequestAction: public Action {
public:
    explicit AddClientRequestAction(ClientRequest* request): Action(CreateClientRequest), request(request) {}

    ClientRequest* request;
};

#endif //P8_WEB_SERVER_ADD_CLIENT_REQUEST_ACTION_H
