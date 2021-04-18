//
// Created by Chris Luttio on 4/14/21.
//

#ifndef P8_WEB_SERVER_HTTP_MESSAGE_PARSER_H
#define P8_WEB_SERVER_HTTP_MESSAGE_PARSER_H

#include "http_message.h"

class HttpMessageParser {
public:

    [[nodiscard]] std::shared_ptr<HttpMessage> get_message() const {
        return message;
    }

private:
    std::shared_ptr<HttpMessage> message;


};


#endif //P8_WEB_SERVER_HTTP_MESSAGE_PARSER_H
