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

    /**
     * @brief Checks if given port is valid and free for socket connection.
     *
     * @param port port to check between 0 - 65535
     * @return true, if port is valid, otherwise false
     */
    bool isPortFree(unsigned int port);

    /**
     * @brief Checks if given string represents ipv4 address
     * @param string string to check
     * @return true, if string represents ipv4 address, otherwise false
     */
    bool isIpAddress(std::string string);
}


#endif //DAMA_SERVER_PARAMETERUTILS_H
