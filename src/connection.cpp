//
// Created by Chris Luttio on 3/25/21.
//

#include "connection.h"

bool Connection::read(std::string *output) {
    int amount = socket.read(output);
    if (amount > 0) {
        last_read = std::chrono::high_resolution_clock::now();
    }
    return amount > 0;
}

bool Connection::write(std::string *input) {
    socket.write(input);
}