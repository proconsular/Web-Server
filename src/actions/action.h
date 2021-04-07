//
// Created by Chris Luttio on 4/4/21.
//

#ifndef P8_WEB_SERVER_ACTION_H
#define P8_WEB_SERVER_ACTION_H

enum Actions {
    None,
    CreateHttpRequest,
    CreateClientConnection,
    ModifyClientConnection,
    RemoveClientConnection,
    CreateServerSocket,
    SetConfiguration,
    CreateClientRequest,
    ModifyClientRequest,
    RemoveClientRequest,
    CreateHttpResponse,
    ClearHttpResponses,
};

class Action {
public:
    explicit Action(Actions type, void* data): type(type), data(data) {}
    explicit Action(Actions type): type(type), data(nullptr) {}

    Actions type;
    void* data;
};

#endif //P8_WEB_SERVER_ACTION_H
