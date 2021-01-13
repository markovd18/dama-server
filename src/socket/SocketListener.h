//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_SOCKETLISTENER_H
#define DAMA_SERVER_SOCKETLISTENER_H

#include <unistd.h>
#include <vector>
#include "../app/config.h"
#include "../app/Connection.h"

/**
 * Class responsible for listening on given port and accepting client requests. This class serves as a connection
 * service.
 */
class SocketListener {
private: //private attributes
    /**
     * File descriptor of server socket.
     */
    int m_serverSocket;
    /**
     * Maximal number of allowed connections at the same time. Default value is 2.
     */
    int m_maxConnectionsAllowed = 2;

    std::vector<app::Connection> m_connections;
    /**
     * First file descriptor index in fd-set to read.
     */
    static constexpr int FIRST_FD_INDEX = 3;
public: //public methods

    /**
     * Creates instance of SocketListener, that listens on 127.0.0.1 on given port. Throws exception when
     * connection could not be established.
     *
     * @param address to listen on
     * @param port port to listen on
     * @param maxConnections maximum number of connections allowed at the same time
     */
    explicit SocketListener(const app::config& configuration);
    /**
     * In destructor we close the open socket.
     */
    ~SocketListener() {
        close(m_serverSocket);
    }
    /**
     * Starts accepting client connections.
     */
    void acceptConnections();

private: // private methods
    void disconnect(int socket, fd_set *clientSockets);
    void processInvalidRequest(int socket, fd_set *clientSockets);
};


#endif
