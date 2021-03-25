//
// Created by Chris Luttio on 3/23/21.
//


#include "socket.h"
#include <iostream>

int Socket::init() {
    return _id = socket(AF_INET, SOCK_STREAM, 0);
}

void Socket::setup(int port) {
    _address.sin_family = AF_INET;
    _address.sin_addr.s_addr = INADDR_ANY;
    _address.sin_port = htons(port);
    _port = port;
}

int Socket::bind() {
    return ::bind(_id, (struct sockaddr*) &_address, sizeof (_address));
}

int Socket::listen(int amount) {
    return ::listen(_id, amount);
}

bool Socket::accept(Socket& socket) {
    int addrlen = sizeof _address;
    int id = ::accept(_id, (struct sockaddr*) &_address, (socklen_t*) &addrlen);
    socket = Socket(id);
    return id >= 0;
}