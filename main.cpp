#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <fcntl.h>

#include "socket.h"

#define PORT 8080

int main() {

    Socket sock;

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if (sock.init() == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    fcntl(sock.id(), F_SETFL, O_NONBLOCK);

    sock.setup(PORT);

    if (sock.bind() < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (sock.listen(3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::vector<Socket> connections;
    std::vector<bool> active;

    while (true) {
        Socket new_client;
        if (sock.accept(new_client)) {
            connections.push_back(new_client);
            active.push_back(true);
            std::cout << "new connection" << std::endl;
        }

        for (int i = 0; i < connections.size(); i++) {
            auto connection = connections[i];

            int error_code;
            int error_code_size = sizeof(error_code);
            getsockopt(connection.id(), SOL_SOCKET, SO_ERROR, &error_code, reinterpret_cast<socklen_t *>(&error_code_size));
            std::cout << error_code << std::endl;

            if (error_code == 0) {
                char buffer[1024] = {0};
                read(connection.id(), buffer, 1024);
                std::cout << buffer << std::endl;

                std::string message = "Message received.";
                send(connection.id(), message.c_str(), message.length(), 0);
                std::cout << "Sent Message" << std::endl;
            } else {
                active[i] = false;
            }
        }

        for (int i = connections.size() - 1; i >= 0; i--) {
            if (!active[i]) {
                connections.erase(connections.begin() + i);
                active.erase(active.begin() + i);
                std::cout << "Removed: " << i << std::endl;
            }
        }
        std::cout << "Active Connections: " << connections.size() << std::endl;

        sleep(1);
    }

    return 0;
}
