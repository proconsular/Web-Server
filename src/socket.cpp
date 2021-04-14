//
// Created by Chris Luttio on 3/23/21.
//


#include "socket.h"
#include <iostream>
#include <algorithm>
#include <math.h>

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

int Socket::read(const std::shared_ptr<std::string>& output) const {
    const int BUFFER_SIZE = 1024;

    int amount_read = 0;
    do {
        char buffer[BUFFER_SIZE] = {0};
        amount_read = ::read(_id, buffer, BUFFER_SIZE);
        output->append(buffer);
    } while (amount_read == BUFFER_SIZE);

    return amount_read;
}

int Socket::write(const std::shared_ptr<std::string>& input) const {
    const int BUFFER_SIZE = 1024;
    const char *data = input->c_str();

    int amount_written = 0;
    int cursor = 0;
    do {
        char buffer[BUFFER_SIZE] = {0};
        int size = fmin(input->size() - cursor, BUFFER_SIZE);
        memcpy(buffer, data + cursor, size);
        amount_written = ::write(_id, buffer, size);
        cursor += amount_written;
    } while (amount_written == BUFFER_SIZE);

    return amount_written;
}

int Socket::get_error() const {
    int error_code;
    int error_code_size = sizeof(error_code);
    getsockopt(_id, SOL_SOCKET, SO_ERROR, &error_code, reinterpret_cast<socklen_t *>(&error_code_size));
    return error_code;
}