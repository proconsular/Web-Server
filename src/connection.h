//
// Created by Chris Luttio on 3/25/21.
//

#ifndef P8_WEB_SERVER_CONNECTION_H
#define P8_WEB_SERVER_CONNECTION_H

#include <chrono>
#include <utility>
#include "socket.h"
#include "utils.h"
#include <fcntl.h>

enum ConnectionType {
    Server,
    Client,
};

class Connection {
public:
    explicit Connection(): alive(true), active_requests(0), persistence(KEEP_ALIVE) {}

    explicit Connection(ConnectionType type, std::string id, Socket socket): _id(std::move(id)), type(type) {
        this->socket = socket;
        fcntl(socket.id, F_SETFL, O_NONBLOCK);
        alive = true;
        last_read = std::chrono::high_resolution_clock::now();
        active_requests = 0;
        persistence = KEEP_ALIVE;
    }

    bool read(const std::shared_ptr<std::string>&);
    bool write(const std::shared_ptr<std::string>&) const;

    void terminate() {
        alive = false;
    }

    std::string id() {
        return _id;
    }

    ConnectionType type;

    enum {
        KEEP_ALIVE,
        CLOSE
    } persistence;

    Socket socket;
    bool alive;

    int active_requests;
    std::chrono::high_resolution_clock::time_point last_read;

private:
    std::string _id;
};


#endif //P8_WEB_SERVER_CONNECTION_H
