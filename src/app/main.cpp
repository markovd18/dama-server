//
// Author: markovd@students.zcu.cz
//

#include "Server.h"
#include "../utils/ParameterUtils.h"

/**
 * Server side of out game.
 */
int main(int argc, char** argv) {

    if (!ParameterUtils::validateCLIparams(argc, argv)) {
        return EXIT_FAILURE;
    }

    Server::run();
    return 0;
}
