//
// Created by Chris Luttio on 3/23/21.
//

#ifndef P8_WEB_SERVER_HTTP_REQUEST_H
#define P8_WEB_SERVER_HTTP_REQUEST_H

#include <string>
#include <map>

#include "url.h"

class HTTPRequest {
public:
    [[nodiscard]] std::shared_ptr<std::string> generate() const;

    std::string method;
    std::string version;
    URL uri;

    std::map<std::string, std::string> headers;
    std::shared_ptr<std::string> body;
private:

};


#endif //P8_WEB_SERVER_HTTP_REQUEST_H
