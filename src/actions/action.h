//
// Created by Chris Luttio on 4/4/21.
//

#ifndef P8_WEB_SERVER_ACTION_H
#define P8_WEB_SERVER_ACTION_H

enum Actions {
    None,
    CreateHttpRequest,
    CreateClientConnection,
    CreateServerSocket,
    SetConfiguration,
    RemoveClientConnection,
    CreateClientRequest,
    ModifyClientRequest,
    CreateHttpResponse,
    ClearHttpResponses,
};

class Action {
public:
    explicit Action(Actions type): type(type) {}

    Actions type;
};

#endif //P8_WEB_SERVER_ACTION_H
