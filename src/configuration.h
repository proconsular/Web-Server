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
    }

    URL base_url;
    int port;
    int keep_alive;

    bool port_fixed;

    std::string from_file;
};

#endif //P8_WEB_SERVER_CONFIGURATION_H
