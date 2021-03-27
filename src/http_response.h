//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_HTTP_RESPONSE_H
#define P8_WEB_SERVER_HTTP_RESPONSE_H

#include <string>
#include <map>

class HTTPResponse {
public:
    HTTPResponse(int code, const std::string& status, const std::string& version) {
        this->code = code;
        this->status = status;
        this->version = version;
    }

    static HTTPResponse OK() {
        return HTTPResponse(200, "OK", "HTTP/1.1");
    }

    int code;
    std::string version;
    std::string status;
    std::map<std::string, std::string> headers;
    std::string body;

    std::string generate();
private:
};


#endif //P8_WEB_SERVER_HTTP_RESPONSE_H
