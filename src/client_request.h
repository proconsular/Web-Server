//
// Created by Chris Luttio on 3/26/21.
//

#ifndef P8_WEB_SERVER_CLIENT_REQUEST_H
#define P8_WEB_SERVER_CLIENT_REQUEST_H

#include <iostream>
#include <map>

#include "connection.h"
#include "url.h"
#include "identifiable.h"
#include "utils.h"

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

class ClientRequest: public Identifiable {
public:
    explicit ClientRequest():
        type(Requests::Unsupported),
        status(RequestStatus::New),
        data(nullptr),
        connection(nullptr) {
        _id = generate_hash_id(10);
    }

    [[nodiscard]] std::string id() const override {
        return _id;
    }

    Requests type;
    RequestStatus status;

    URL uri;
    URL path;
    std::string* data;

    Connection* connection;

private:
    std::string _id;
};


#endif //P8_WEB_SERVER_CLIENT_REQUEST_H
