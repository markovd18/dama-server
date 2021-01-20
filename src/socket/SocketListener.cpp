//
// Author: markovd@students.zcu.cz
//

#define BUFFER_LENGTH 100
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <asm/ioctls.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <algorithm>
#include "SocketListener.h"

#include "../exception/ConnectionException.h"
#include "../utils/ParameterUtils.h"
#include "../utils/Logger.h"
#include "../request/RequestHandler.h"

SocketListener::SocketListener(app::config& configuration) : m_maxConnectionsAllowed(configuration.maxConnections) {
    if (!app::isIpAddress(configuration.address)) {
        throw std::invalid_argument("Given address is invalid!");
    }
    if (!app::isPortFree(configuration.port)) {
        throw std::invalid_argument("Given port is reserved for another service!");
    }

    m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_serverSocket < 0) {
        throw ConnectionException("Error while creating server socket");
    }

    int enableOpt = 1;
    if (setsockopt(m_serverSocket, SOL_SOCKET, SO_REUSEADDR, &enableOpt, sizeof(int)) < 0) {
        app::Logger::getInstance().warn("Enabling address re-usability failed!");
    }

    if (configuration.address == "localhost") {
        configuration.address = "127.0.0.1";
    }

    struct sockaddr_in myAddress {};
    myAddress.sin_port = htons(configuration.port);
    myAddress.sin_family = AF_INET;
    myAddress.sin_addr.s_addr = inet_addr(configuration.address.c_str());

    auto bound = bind(m_serverSocket, (struct sockaddr*) &myAddress, sizeof(struct sockaddr_in));
    if (bound != 0) {
        throw ConnectionException("Error while binding server socket to address");
    }

    auto listening = ::listen(m_serverSocket, m_maxConnectionsAllowed);
    if (listening < 0) {
        throw ConnectionException("Error while trying to start listening on server socket");
    }

    //TODO markovda process max games info
    app::Logger::getInstance().info(std::string("Listenning at " )
            + configuration.address + ":" + std::to_string(configuration.port));
    app::Logger::getInstance().info(std::string("Configured with ") + std::to_string(configuration.maxConnections)
            + " max connections and " + std::to_string(configuration.maxGames) + " max games at the same time.");
}

void SocketListener::acceptConnections() {

    fd_set clientSockets, sideSockets;

    FD_ZERO(&clientSockets);
    FD_SET(m_serverSocket, &clientSockets);

    socklen_t addressLength;
    struct sockaddr_in peerAddress{};

    app::RequestHandler requestHandler{};
    while (true) {
        sideSockets = clientSockets;
        int returnValue = select(FD_SETSIZE, &sideSockets, (fd_set *) nullptr, (fd_set *) nullptr, (struct timeval *) nullptr);
        if (returnValue < 0) {
            app::Logger::getInstance().error("Error while selecting from sockets");
            return;
        }

        for (int fileDescriptor = FIRST_FD_INDEX; fileDescriptor < FD_SETSIZE; ++fileDescriptor) {
            if (!FD_ISSET(fileDescriptor, &sideSockets)) {
                continue;
            }

            if (fileDescriptor == m_serverSocket) {
                /// We found server socket - there is new incoming connection
                int clientSocket = accept(m_serverSocket,(struct sockaddr*) &peerAddress, &addressLength);
                if (clientSocket < 0) {
                    app::Logger::getInstance().error("Error crating communication socket!");
                    continue;
                }

                FD_SET(clientSocket, &clientSockets);
                m_connectionService.addNewConnection(clientSocket);
                app::Logger::getInstance().info("New client connection");
            } else {
                /// We found client socket - there is incoming client message
                int bytesToRead;
                ioctl(fileDescriptor, FIONREAD, &bytesToRead);

                if (bytesToRead > 0) {
                    /// Incomming message
                    std::array<char, BUFFER_LENGTH> buffer { 0 };
                    recv(fileDescriptor, buffer.data(), BUFFER_LENGTH, 0);
                    app::Logger::getInstance().debug(std::string("Incomming message: ") + buffer.data());

                    app::Response response(requestHandler.processRequest(buffer.data(), fileDescriptor));
                    /**
                     * Check for login attempt has to be here, because here we know from which socket the request came from.
                     */
                    if ((response.getMessage().find(std::to_string(app::Response::NEW_LOGIN_OK)) == 0) ||
                        (response.getMessage().find(std::to_string(app::Response::RECONNECT_OK)) == 0)) {
                        m_connectionService.findConnection(fileDescriptor)->setUserId(atoi(app::parseString(response.getMessage(), "|")[1].c_str()));
                    }
                    m_connectionService.sendResponse(response, fileDescriptor);

                    if (!response.wasRequestValid()) {
                        bool disconnected = m_connectionService.processInvalidRequest(fileDescriptor);
                        if (disconnected) {
                            FD_CLR(fileDescriptor, &clientSockets);
                            close(fileDescriptor);
                            app::Logger::getInstance().info("Client disconnected!");
                        }
                    }
                } else {
                    /// Client disconnected
                    disconnect(fileDescriptor, &clientSockets);
                }
            }
        }
    }
}

void SocketListener::disconnect(int socket, fd_set *clientSockets) {
    m_connectionService.disconnect(socket);
    close(socket);
    FD_CLR(socket, clientSockets);
    app::Logger::getInstance().info("Client disconnected");
}
