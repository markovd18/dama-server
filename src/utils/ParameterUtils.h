//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_PARAMETERUTILS_H
#define DAMA_SERVER_PARAMETERUTILS_H

#include <string>

/**
 * Utility class for working with parameters and their validation.
 */
namespace app {
    /**
     *@brief Validates parameters from CLI.
     *
     * @param params CLI parameters
     * @throw invalid_argument if arguments are invalid
     */
    void validateCLIparams(int argc, char** params);
}


#endif //DAMA_SERVER_PARAMETERUTILS_H
