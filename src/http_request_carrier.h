//
// Created by Chris Luttio on 3/31/21.
//

#ifndef P8_WEB_SERVER_HTTP_REQUEST_CARRIER_H
#define P8_WEB_SERVER_HTTP_REQUEST_CARRIER_H

#include <utility>

#include "http_request.h"
#include "connection.h"

enum HTTPRequestStatus {
    NEW,
    CONNECTED,
    SENT,
    FULFILLED,
    FAILED
};

class HTTPRequestCarrier {
public:
    explicit HTTPRequestCarrier(URL url, HTTPRequest* request): url(std::move(url)), http_request(request), status(NEW) {
        connection = nullptr;
    }

    HTTPRequestStatus status;
    HTTPRequest* http_request;
    Connection* connection;

    URL url;
};


#endif //P8_WEB_SERVER_HTTP_REQUEST_CARRIER_H
