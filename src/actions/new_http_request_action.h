//
// Created by Chris Luttio on 4/4/21.
//

#ifndef P8_WEB_SERVER_NEW_HTTP_REQUEST_ACTION_H
#define P8_WEB_SERVER_NEW_HTTP_REQUEST_ACTION_H

#include "action.h"
#include "http_request.h"
#include "http_request_envelope.h"

class NewHttpRequestAction: public Action {
public:
    explicit NewHttpRequestAction(HTTPRequestEnvelope http_request): Action(CreateHttpRequest), http_request_envelope(http_request) {}

    HTTPRequestEnvelope http_request_envelope;
};

#endif //P8_WEB_SERVER_NEW_HTTP_REQUEST_ACTION_H
