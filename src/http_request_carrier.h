//
// Created by Chris Luttio on 3/31/21.
//

#ifndef P8_WEB_SERVER_HTTP_REQUEST_CARRIER_H
#define P8_WEB_SERVER_HTTP_REQUEST_CARRIER_H

#include <utility>

#include "http_request.h"
#include "connection.h"
#include <memory>
#include "http_response.h"

enum HTTPRequestStatus {
    NEW,
    CONNECTED,
    SENT,
    FULFILLED,
    FAILED
};

class HTTPRequestCarrier {
public:
    explicit HTTPRequestCarrier(URL url, std::shared_ptr<HTTPRequest> request): url(std::move(url)), http_request(std::move(request)), status(NEW) {
        connection = nullptr;
        _id = generate_hash_id(10);
    }

    [[nodiscard]] std::string id() const {
        return _id;
    }

    HTTPRequestStatus status;
    std::shared_ptr<HTTPRequest> http_request;
    std::shared_ptr<Connection> connection;

    std::shared_ptr<HTTPResponse> http_response;

    URL url;

private:
    std::string _id;
};


#endif //P8_WEB_SERVER_HTTP_REQUEST_CARRIER_H
