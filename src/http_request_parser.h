//
// Created by Chris Luttio on 3/23/21.
//

#ifndef P8_WEB_SERVER_HTTP_REQUEST_PARSER_H
#define P8_WEB_SERVER_HTTP_REQUEST_PARSER_H

#include "http_request.h"

class HTTPRequestParser {
public:
    static HTTPRequest* parse(const std::string&);

private:
    static void processRequestLine(HTTPRequest*, const std::string&, std::string::const_iterator&);
    static void processHeaders(HTTPRequest*, const std::string&, std::string::const_iterator&);
    static void next(std::string::const_iterator& i, const std::string& data) {
        while (i != data.end() && (*i == '\r' || *i == '\n')) ++i;
    }
};


#endif //P8_WEB_SERVER_HTTP_REQUEST_PARSER_H
