//
// Created by Chris Luttio on 3/25/21.
//

#include "connection.h"

bool Connection::read(const std::shared_ptr<std::string>& output) {
    int amount = socket.read(output);
    if (amount > 0) {
        last_read = std::chrono::high_resolution_clock::now();
    }
    return amount > 0;
}

bool Connection::write(const std::shared_ptr<std::string>& input) const {
    return socket.write(input) >= 0;
}