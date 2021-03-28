//
// Created by Chris Luttio on 3/27/21.
//

#ifndef P8_WEB_SERVER_HTTP_REQUEST_ENVELOPE_H
#define P8_WEB_SERVER_HTTP_REQUEST_ENVELOPE_H

#include "http_request.h"
#include "connection.h"

class HTTPRequestEnvelope {
public:
    HTTPRequestEnvelope(Connection* conn, HTTPRequest* http_request): connection(conn), request(http_request) {}

    Connection* connection;
    HTTPRequest* request;
};

#endif //P8_WEB_SERVER_HTTP_REQUEST_ENVELOPE_H
