//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_CONNECTION_H
#define P8_WEB_SERVER_CONNECTION_H

#include <chrono>
#include "socket.h"
#include "utils.h"

class Connection {
public:
    explicit Connection(Socket socket) {
        this->socket = socket;
        alive = true;
        last_read = std::chrono::high_resolution_clock::now();
        active_requests = 0;
        _id = generate_hash_id(10);
    }

    bool read(std::string*);
    bool write(std::string*);

    void terminate() {
        alive = false;
    }

    std::string id() {
        return _id;
    }

    Socket socket;
    bool alive;

    int active_requests;
    std::chrono::high_resolution_clock::time_point last_read;

private:
    std::string _id;
};


#endif //P8_WEB_SERVER_CONNECTION_H
