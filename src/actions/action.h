#include <utility>

//
// Created by Chris Luttio on 4/4/21.
//

#ifndef P8_WEB_SERVER_ACTION_H
#define P8_WEB_SERVER_ACTION_H

#include <memory>

enum Actions {
    None,

    StartProgram,

    CreateHttpRequest,
    ClearHttpRequests,
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

    CreateOutboundHttpRequest,
    InitializeHttpRequestConnection,
    SendHttpRequest,
    ModifyHttpCarrier,
    ReceiveHttpResponse,

    ReportError,
    ReportLog,
};

class Action {
public:
    explicit Action(Actions type, std::shared_ptr<void> data): type(type), data(std::move(data)) {}
    explicit Action(Actions type): type(type), data(nullptr) {}

    Actions type;
    std::shared_ptr<void> data;
};

#endif //P8_WEB_SERVER_ACTION_H
