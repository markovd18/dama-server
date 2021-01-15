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
#include <uv.h>
#include <string>
#include <algorithm>
#include "SocketListener.h"

#include "../exception/ConnectionException.h"
#include "../utils/ParameterUtils.h"
#include "../utils/Logger.h"
#include "../request/RequestHandler.h"

SocketListener::SocketListener(const app::config& configuration) : m_maxConnectionsAllowed(configuration.maxConnections) {
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

    //TODO markovda process mac games info
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
                m_connections.emplace_back(app::Connection(clientSocket));
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

                    app::Response response(requestHandler.processRequest(buffer.data()));
                    sendResponse(response, fileDescriptor);
                    if (!response.wasRequestValid()) {
                        processInvalidRequest(fileDescriptor, &clientSockets);
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
    for (int i = 0; i < m_connections.size(); ++i) {
        if (m_connections[i].getSocket() == socket) {
            if (m_connections[i].getUserId() != 0) {
                //m_playerService.setPlayerState(m_connections[i].getUserId(), app::PlayerState::DISCONNECTED);
            }
            m_connections.erase(m_connections.begin() + i);
            close(socket);
            FD_CLR(socket, clientSockets);
            app::Logger::getInstance().info("Client disconnected");
            break;
        }
    }
}

void SocketListener::processInvalidRequest(int socket, fd_set *clientSockets) {
    for (int i = 0; i < m_connections.size(); ++i) {
        if (m_connections[i].getSocket() == socket) {
            m_connections[i].incErrCount();
            if (m_connections[i].getErrorCount() == app::Connection::MAX_ERROR_COUNT) {
                if (m_connections[i].getUserId() != 0) {
                    // TODO markovda remove/set state to dc player
                }
                m_connections.erase(m_connections.begin() + i);
                close(socket);
                FD_CLR(socket, clientSockets);
                app::Logger::getInstance().info("Client disconnected");
            }
            break;
        }
    }
}

void SocketListener::sendResponse(const app::Response &response, const int socket) {
    if (response.getMessage().find(std::to_string(app::Response::NEW_LOGIN_OK)) == 0) {
        for (auto &connection : m_connections) {
            if (connection.getSocket() == socket) {
                connection.setUserId(atoi(app::parseString(response.getMessage(), "|")[1].c_str()));
                break;
            }
        }
    } else if (response.getMessage().find(std::to_string(app::Response::LOGOUT_OK)) == 0) {
        for (auto &connection : m_connections) {
            if (connection.getSocket() == socket) {
                connection.setUserId(0);
                break;
            }
        }
    } else if (response.getMessage().find(std::to_string(app::Response::CREATE_GAME_OK)) == 0) {
        // TODO markovda send broadcast
    }
    app::Logger::getInstance().debug(std::string("Sending message: ") + response.getMessage() + " through socket " + std::to_string(socket));

    if (send(socket, response.getMessage().c_str(), response.getMessage().size(), 0) < 0) {
        app::Logger::getInstance().error(
                std::string("Error while sending message: ") + response.getMessage() + "through socket " + std::to_string(socket));
    }
}