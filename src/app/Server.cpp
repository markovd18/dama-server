//
// Author: markovd@students.zcu.cz
//

#include "Server.h"
#include "../socket/SocketListener.h"

void Server::run() {
    SocketListener listener(PORT);
    listener.acceptConnections();
}