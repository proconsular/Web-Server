//
// Created by Chris Luttio on 4/24/21.
//

#ifndef P8_WEB_SERVER_ROUTE_H
#define P8_WEB_SERVER_ROUTE_H

#include <iostream>
#include <map>

class Route {
public:

    std::string url;
    std::string default_file;

    std::map<std::string, std::string> content_type;
    std::map<std::string, std::string> language;

    std::string cache_control;
};


#endif //P8_WEB_SERVER_ROUTE_H
