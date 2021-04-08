//
// Created by Chris Luttio on 3/23/21.
//

#ifndef P8_WEB_SERVER_HTTP_REQUEST_PARSER_H
#define P8_WEB_SERVER_HTTP_REQUEST_PARSER_H

#include "http_request.h"

class HTTPRequestParser {
public:
    static std::shared_ptr<HTTPRequest> parse(const std::shared_ptr<std::string>&);

private:
    static void processRequestLine(const std::shared_ptr<HTTPRequest>&, const std::shared_ptr<std::string>&, std::string::const_iterator&);
    static void processHeaders(const std::shared_ptr<HTTPRequest>&, const std::shared_ptr<std::string>&, std::string::const_iterator&);
    static void next(std::string::const_iterator& i, const std::shared_ptr<std::string>& data) {
        while (i != data->end() && (*i == '\r' || *i == '\n')) ++i;
    }
};


#endif //P8_WEB_SERVER_HTTP_REQUEST_PARSER_H
