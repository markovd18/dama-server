//
// Author: markovd@students.zcu.cz
//

#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include "../app/config.h"
#include "ParameterUtils.h"

constexpr unsigned int FTP_DATA_PORT = 20;
constexpr unsigned int FTP_CONTROL_PORT = 21;
constexpr unsigned int SSH_PORT = 22;
constexpr unsigned int SMTP_PORT = 25;
constexpr unsigned int DNS_PORT = 53;
constexpr unsigned int HTTP_PORT = 80;
constexpr unsigned int POP3_PORT = 110;
constexpr unsigned int SSL_PORT = 443;

constexpr unsigned int MIN_CONNECTIONS = 2;
constexpr unsigned int MIN_PARAMS_NUM = 5;
constexpr unsigned int MIN_GAMES = 1;

constexpr unsigned int MAX_NICK_LENGTH = 20;

namespace app {

    app::config parseCliParams(int argc, char** params) {
        if (argc < MIN_PARAMS_NUM) {
            throw std::invalid_argument("Invalid argument count!");
        }
        if (!isIpAddress(params[1])) {
            throw std::invalid_argument("Invalid IP address passed!");
        }
        unsigned int port;
        if (!isNumber(params[2]) || ((port = atoi(params[2])) < 0)) {
            throw std::invalid_argument("Invalid port number passed!");
        }
        int maxConnections;
        if (!isNumber(params[3]) || ((maxConnections = atoi(params[3])) < MIN_CONNECTIONS)) {
            throw std::invalid_argument("Invalid number of max connections passed!");
        }
        int maxGames;
        if (!isNumber(params[4]) || ((maxGames = atoi(params[4])) < MIN_GAMES)) {
            throw std::invalid_argument("Invalid number of max connections passed!");
        }

        app::config configuration { params[1], port, maxConnections, maxGames};
        return configuration;
    }

    bool isPortFree(const unsigned int port) {
        switch (port) {
            case FTP_DATA_PORT:
            case FTP_CONTROL_PORT:
            case SSH_PORT:
            case SMTP_PORT:
            case DNS_PORT:
            case HTTP_PORT:
            case POP3_PORT:
            case SSL_PORT:
                return false;
            default:
                return true;
        }
    }

    bool isIpAddress(const std::string& string) {
        std::vector<std::string> tokens(parseString(string, "."));
        if (tokens.size() != 4) {
            return false;
        }

        for (const auto &token : tokens) {
            if (token.empty() || token.size() > 3) {
                return false;
            }
            for (const auto &digit : token) {
                if (!isdigit(digit)) {
                    return false;
                }
            }
            int value = atoi(token.c_str());
            if (value > 255 || value < 0) {
                return false;
            }
        }

        return true;
    }

    std::vector<std::string> parseString(std::string string, const std::string& delimiter) {
        std::size_t pos;
        std::vector<std::string> tokens;

        while ((pos = string.find(delimiter)) != std::string::npos) {
            tokens.push_back(trimString(string.substr(0, pos)));
            string.erase(0, pos + 1);
        }

        tokens.push_back(trimString(string));

        return tokens;
    }

    bool isNumber(const std::string& string) {
        if (string.empty()) {
            return false;
        }

        return std::all_of(string.begin(), string.end(), [](const unsigned char c) { return isdigit(c); });
    }

    bool isValidNickname(const std::string& nickname) {
        if (nickname.size() > MAX_NICK_LENGTH) {
            return false;
        }

        return std::all_of(nickname.begin(), nickname.end(), [](const unsigned char c) {
            return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= 0 && c <= 9));
        });
    }

    std::string trimString(const std::string& string) {
        const char* whitespaces = " \n\t\r";

        const auto strBegin = string.find_first_not_of(whitespaces);
        if (strBegin == std::string::npos)
            return ""; // no content

        const auto strEnd = string.find_last_not_of(whitespaces);
        const auto strRange = strEnd - strBegin + 1;

        return string.substr(strBegin, strRange);
    }

    float distance(int ax, int ay, int bx, int by) {
        return std::sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
    }
}

