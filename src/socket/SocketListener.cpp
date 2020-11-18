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
    struct sockaddr_in peerAddress;

    while (true) {
        sideSockets = clientSockets;
        int returnValue = select(FD_SETSIZE, &sideSockets, (fd_set *) 0, (fd_set *) 0, (struct timeval *) 0);
        if (returnValue < 0) {
            std::cout << "Error while selecting from sockets";
            return;
        }

        for (int f_desc = FIRST_FD_INDEX; f_desc < FD_SETSIZE; ++f_desc) {
            if (FD_ISSET(f_desc, &sideSockets)) {
                if (f_desc == serverSocket) {
                    int clientSocket = accept(serverSocket,(struct sockaddr*) &peerAddress, &addressLength);
                    if (clientSocket < 0) {
                        std::cout << "Error creating communication socket!\n";
                        continue;
                    }

                    FD_SET(clientSocket, &clientSockets);
                    std::cout << "New client connection\n";
                } else {
                    // client socket
                    int addressToRead;
                    ioctl(f_desc, FIONREAD, &addressToRead);

                    if (addressToRead > 0) {

                        //TODO obsluha
                    }
                    // we disconnect the client even if he answers correctly
                    close(f_desc);
                    FD_CLR(f_desc, &clientSockets);
                    std::cout << "Client disconnected\n";
                }
            }
        }
    }
}