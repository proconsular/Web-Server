//
// Created by Chris Luttio on 3/23/21.
//

#ifndef P8_WEB_SERVER_SOCKET_H
#define P8_WEB_SERVER_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>

class Socket {
public:
    Socket() {}
    Socket(int id): _id(id) {}

    int init();
    int bind();
    int listen(int);

    int get_error() const;

    int read(const std::shared_ptr<std::string>&) const;
    int write(const std::shared_ptr<std::string>&) const;

    bool accept(Socket&);

    void setup(int);

    int id() const {
        return _id;
    }

    int port() const {
        return _port;
    }
private:
    int _id, _port;
    struct sockaddr_in _address;
};

#endif //P8_WEB_SERVER_SOCKET_H
