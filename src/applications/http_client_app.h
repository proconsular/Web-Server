//
// Created by Chris Luttio on 4/15/21.
//

#ifndef P8_WEB_SERVER_HTTP_CLIENT_APP_H
#define P8_WEB_SERVER_HTTP_CLIENT_APP_H

#include <utility>
#include <memory>

#include "http_request.h"
#include "http_response.h"

class HttpClientApp {
public:
    explicit HttpClientApp() = default;
    explicit HttpClientApp(std::string  log): _log_filename(std::move(log)) {}

    bool send(const std::string&, const std::shared_ptr<HTTPRequest>&, std::shared_ptr<HTTPResponse>&);
private:
    std::string _log_filename;
};


#endif //P8_WEB_SERVER_HTTP_CLIENT_APP_H
