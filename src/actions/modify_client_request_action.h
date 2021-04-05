//
// Created by Chris Luttio on 4/5/21.
//

#ifndef P8_WEB_SERVER_MODIFY_CLIENT_REQUEST_ACTION_H
#define P8_WEB_SERVER_MODIFY_CLIENT_REQUEST_ACTION_H

#include "action.h"
#include "client_request.h"

class ModifyClientRequestAction: public Action {
public:
    explicit ModifyClientRequestAction(ClientRequest* request): Action(ModifyClientRequest), request(request) {}

    ClientRequest* request;
};

#endif //P8_WEB_SERVER_MODIFY_CLIENT_REQUEST_ACTION_H
