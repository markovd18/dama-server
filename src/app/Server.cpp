//
// Author: markovd@students.zcu.cz
//

#include <stdexcept>
#include "Server.h"
#include "../socket/SocketListener.h"
#include "../utils/Logger.h"

void Server::run(const std::string& address, const unsigned int port) {
    try {
        SocketListener listener(address, port);
        listener.acceptConnections();
    } catch (const std::invalid_argument& e) {
        app::Logger::getInstance().error(e.what());
    }
}