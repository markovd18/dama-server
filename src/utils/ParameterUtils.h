//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_PARAMETERUTILS_H
#define DAMA_SERVER_PARAMETERUTILS_H

#include <string>
#include <vector>
#include "../app/config.h"

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
    app::config parseCliParams(int argc, char** params);

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
    bool isIpAddress(const std::string& string);

    /**
     * @brief Parses @a string by @a delimiter. If delimiter is the ending part of the string, the following
     * "empty" char will be returned too.
     *
     * @param string string to parse
     * @param delimiter delimiter to parse by
     * @return vector of individual tokens between delimiters
     */
    std::vector<std::string> parseString(std::string string, const std::string& delimiter);

    /**
     * Checks if given string represents a number.
     */
    bool isNumber(const std::string& string);

    /**
     * Checks if given string is valid nickname. Nickname can only has a-zA-Z0-9 characters and maximum of 20.
     *
     * @param nickname nickname to check
     * @return true, if nickname is valid, otherwise false
     */
    bool isValidNickname(const std::string& nickname);
}


#endif //DAMA_SERVER_PARAMETERUTILS_H
