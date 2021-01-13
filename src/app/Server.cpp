//
// Author: markovd@students.zcu.cz
//

#include <stdexcept>
#include "Server.h"
#include "config.h"
#include "../socket/SocketListener.h"
#include "../utils/Logger.h"
#include "../utils/ParameterUtils.h"

void Server::run(const int argc, char** argv) {
    app::Logger::getInstance().info("Starting server instance...");
    app::config configuration;
    try {
        configuration = app::parseCliParams(argc, argv);
    } catch (const std::invalid_argument &ex) {
        app::Logger::getInstance().error(std::string("Error validating input parameters: ") + ex.what());
        return;
    }

    try {
        SocketListener listener(configuration);
        listener.acceptConnections();
    } catch (const std::invalid_argument& e) {
        app::Logger::getInstance().error(e.what());
    }
}