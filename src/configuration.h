//
// Created by Chris Luttio on 3/28/21.
//

#ifndef P8_WEB_SERVER_CONFIGURATION_H
#define P8_WEB_SERVER_CONFIGURATION_H

#include <iostream>
#include "url.h"

class Configuration {
public:
    Configuration() {
        port = 8000;
        keep_alive = 30;
        port_fixed = false;
        base_url = URL::parse("/");
        http_response_read_wait_ms = 5000;
    }

    URL base_url;
    int port;
    int keep_alive;

    uint32_t http_response_read_wait_ms;

    bool port_fixed;

    std::string from_file;
};

#endif //P8_WEB_SERVER_CONFIGURATION_H
