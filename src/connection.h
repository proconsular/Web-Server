//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_CONNECTION_H
#define P8_WEB_SERVER_CONNECTION_H

#include <chrono>
#include "socket.h"

class Connection {
public:
    Connection(Socket socket) {
        this->socket = socket;
        alive = true;
        last_read = std::chrono::high_resolution_clock::now();
    }

    Socket socket;
    bool alive;

    std::chrono::high_resolution_clock::time_point last_read;
};


#endif //P8_WEB_SERVER_CONNECTION_H
