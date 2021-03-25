//
// Created by Chris Luttio on 3/23/21.
//

#ifndef P8_WEB_SERVER_HTTP_REQUEST_H
#define P8_WEB_SERVER_HTTP_REQUEST_H

#include <string>
#include <map>

class HTTPRequest {
public:

    std::string method;
    std::string version;
    std::string uri;

    std::map<std::string, std::string> headers;
private:

};


#endif //P8_WEB_SERVER_HTTP_REQUEST_H
