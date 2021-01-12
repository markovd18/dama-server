//
// Author: markovd@students.zcu.cz
//

#include <stdexcept>
#include <iostream>
#include "Server.h"
#include "../utils/ParameterUtils.h"
#include "../utils/Logger.h"

/**
 * Server side of our game.
 */
int main(int argc, char** argv) {
    app::Logger::getInstance().info("Starting server instance...");

    try {
        app::validateCLIparams(argc, argv);
    } catch (const std::invalid_argument &ex) {
        app::Logger::getInstance().error("Error validating input parameters:");
        app::Logger::getInstance().error(ex.what());
    }

    Server::run(argv[1], atoi(argv[2]));
    return 0;
}
