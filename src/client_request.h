//
// Created by Chris Luttio on 3/26/21.
//

#ifndef P8_WEB_SERVER_CLIENT_REQUEST_H
#define P8_WEB_SERVER_CLIENT_REQUEST_H

#include <iostream>
#include <map>

#include "connection.h"
#include "url.h"

enum Requests {
    Unsupported,
    RetrieveFile,
};

enum RequestStatus {
    New,
    Working,
    Complete,
    Failed,
};

class ClientRequest {
public:
    ClientRequest(): type(Requests::Unsupported), status(RequestStatus::New) {}

    Requests type;
    RequestStatus status;

    URL uri;
    URL path;
    std::string* data;

    Connection* connection;
};


#endif //P8_WEB_SERVER_CLIENT_REQUEST_H
