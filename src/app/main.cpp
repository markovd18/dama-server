//
// Author: markovd@students.zcu.cz
//

#include <stdexcept>
#include <iostream>
#include "Server.h"
#include "../utils/ParameterUtils.h"

/**
 * Server side of our game.
 */
int main(int argc, char** argv) {

    try {
        app::validateCLIparams(argc, argv);
    } catch (const std::invalid_argument &ex) {
        std::cout << "Error validating input parameters:\n" << ex.what() << std::endl;
    }

    Server::run();
    return 0;
}
