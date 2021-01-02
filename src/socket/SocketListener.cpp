//
// Author: markovd@students.zcu.cz
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <asm/ioctls.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "SocketListener.h"

#include "../exception/ConnectionException.h"

SocketListener::SocketListener(int port) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        throw ConnectionException("Error while creating server socket");
    }

    struct sockaddr_in myAddres {};
    myAddres.sin_port = htons(port);
    myAddres.sin_family = AF_INET;
    myAddres.sin_addr.s_addr = INADDR_ANY;

    auto bound = bind(serverSocket, (struct sockaddr*) &myAddres, sizeof(struct sockaddr_in));
    if (bound != 0) {
        throw ConnectionException("Error while binding server socket to address");
    }

    auto listening = ::listen(serverSocket, 5);
    if (listening < 0) {
        throw ConnectionException("Error while trying to start listening on server socket");
    }
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
            std::cout << "Error while selecting from sockets";
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
                    std::cout << "Error creating communication socket!\n";
                    continue;
                }

                FD_SET(clientSocket, &clientSockets);
                std::cout << "New client connection\n";
            } else {
                /// We found client socket - there is incoming client message
                int addressToRead;
                int returnVal = ioctl(fileDescriptor, FIONREAD, &addressToRead);
                std::cout << returnVal << std::endl;

                if (addressToRead > 0) {
                    //TODO markovda process client message
                } else {
                    close(fileDescriptor);
                    FD_CLR(fileDescriptor, &clientSockets);
                    std::cout << "Error on socket, client disconnected" << std::endl;
                }
            }
        }
    }
}