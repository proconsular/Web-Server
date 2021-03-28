//
// Created by Chris Luttio on 3/27/21.
//

#ifndef P8_WEB_SERVER_HTTP_RESPONSE_ENVELOPE_H
#define P8_WEB_SERVER_HTTP_RESPONSE_ENVELOPE_H

#include "connection.h"
#include "http_response.h"

class HTTPResponseEnvelope {
public:
    HTTPResponseEnvelope(Connection* conn, HTTPResponse* response): connection(conn), response(response) {}

    Connection* connection;
    HTTPResponse* response;
};

#endif //P8_WEB_SERVER_HTTP_RESPONSE_ENVELOPE_H
