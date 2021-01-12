//
// Author: markovd@students.zcu.cz
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <asm/ioctls.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <uv.h>
#include "SocketListener.h"

#include "../exception/ConnectionException.h"
#include "../utils/ParameterUtils.h"
#include "../utils/Logger.h"

SocketListener::SocketListener(const std::string& address, const unsigned int port) {
    if (!app::isIpAddress(address)) {
        throw std::invalid_argument("Given address is invalid!");
    }
    if (!app::isPortFree(port)) {
        throw std::invalid_argument("Given port is reserved for another service!");
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw ConnectionException("Error while creating server socket");
    }

    int enableOpt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enableOpt, sizeof(int)) < 0) {
        app::Logger::getInstance().warn("Enabling address re-usability failed!");
    }

    struct sockaddr_in myAddress {};
    myAddress.sin_port = htons(port);
    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = inet_addr(address.c_str());

    auto bound = bind(serverSocket, (struct sockaddr*) &myAddress, sizeof(struct sockaddr_in));
    if (bound != 0) {
        throw ConnectionException("Error while binding server socket to address");
    }

    auto listening = ::listen(serverSocket, 5);
    if (listening < 0) {
        throw ConnectionException("Error while trying to start listening on server socket");
    }

    app::Logger::getInstance().info(std::string("Listenning at " ) + address + ":" + std::to_string(port));
}

void SocketListener::acceptConnections() {

    fd_set clientSockets, sideSockets;

    FD_ZERO(&clientSockets);
    FD_SET(serverSocket, &clientSockets);

    socklen_t addressLength;
    struct sockaddr_in peerAddress{};

    while (true) {
        sideSockets = clientSockets;
        int returnValue = select(FD_SETSIZE, &sideSockets, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);
        if (returnValue < 0) {
            app::Logger::getInstance().error("Error while selecting from sockets");
            return;
        }

        for (int fileDescriptor = FIRST_FD_INDEX; fileDescriptor < FD_SETSIZE; ++fileDescriptor) {
            if (!FD_ISSET(fileDescriptor, &sideSockets)) {
                continue;
            }

            if (fileDescriptor == serverSocket) {
                /// We found server socket - there is new incoming connection
                int clientSocket = accept(serverSocket,(struct sockaddr*) &peerAddress, &addressLength);
                if (clientSocket < 0) {
                    app::Logger::getInstance().error("Error crating communication socket!");
                    continue;
                }

                FD_SET(clientSocket, &clientSockets);
                app::Logger::getInstance().info("New client connection");
            } else {
                /// We found client socket - there is incoming client message
                int bytesToRead;
                ioctl(fileDescriptor, FIONREAD, &bytesToRead);

                if (bytesToRead > 0) {
                    //TODO markovda process client message
                    char buffer[25];
                    recv(fileDescriptor, buffer, 25, 0);
                    app::Logger::getInstance().debug(std::string("Incomming message: ") + buffer);
                } else {
                    close(fileDescriptor);
                    FD_CLR(fileDescriptor, &clientSockets);
                    app::Logger::getInstance().info("Client disconnected");
                }
            }
        }
    }
}