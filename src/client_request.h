//
// Created by Chris Luttio on 3/26/21.
//

#ifndef P8_WEB_SERVER_CLIENT_REQUEST_H
#define P8_WEB_SERVER_CLIENT_REQUEST_H

#include <iostream>

enum Requests {
    Unsupported,
    RetrieveFile,
};

class ClientRequest {
public:

    Requests type;
    std::string uri;
    std::string data;
};


#endif //P8_WEB_SERVER_CLIENT_REQUEST_H
