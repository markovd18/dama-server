//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_SOCKETLISTENER_H
#define DAMA_SERVER_SOCKETLISTENER_H

#include <unistd.h>
/**
 * Class responsible for listening on given port and accepting client requests.
 */
class SocketListener {
private: //private attributes
    /**
     * File descriptor of server socket.
     */
    int serverSocket;
    /**
     * First file descriptor index in fd-set to read.
     */
    static constexpr int FIRST_FD_INDEX = 3;
public: //public methods

    /**
     * Creates instance of SocketListener, that listens on 127.0.0.1 on given port. Throws exception when
     * connection could not be established.
     *
     * @param port port to listen on
     */
    explicit SocketListener(int port);
    /**
     * In destructor we close the open socket.
     */
    ~SocketListener() {
        close(serverSocket);
    }
    /**
     * Starts accepting client connections.
     */
    void acceptConnections();
};


#endif
