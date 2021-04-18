//
// Created by Chris Luttio on 4/14/21.
//

#ifndef P8_WEB_SERVER_HTTP_MESSAGE_H
#define P8_WEB_SERVER_HTTP_MESSAGE_H

#include "url.h"

#include <string>
#include <map>
#include <memory>

enum HttpMessageType {
    REQUEST,
    RESPONSE,
};

class HttpMessage {
public:
    explicit HttpMessage(HttpMessageType type): type(type), code(0) {}

    HttpMessageType type;

    int code;
    std::string status;
    std::string method;
    URL url;

    std::map<std::string, std::shared_ptr<std::string>> headers;
    std::shared_ptr<std::string> body;
};


#endif //P8_WEB_SERVER_HTTP_MESSAGE_H
