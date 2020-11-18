//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_PARAMETERUTILS_H
#define DAMA_SERVER_PARAMETERUTILS_H

#include <string>

/**
 * Utility class for working with parameters and their validation.
 */
class ParameterUtils {
public:
    /**
     * Validates parameters from CLI.
     *
     * @param params CLI parameters
     * @return true when parameters are valid, otherwise false
     */
    static bool validateCLIparams(int argc, char** params);
};


#endif //DAMA_SERVER_PARAMETERUTILS_H
