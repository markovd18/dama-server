//
// Author: markovd@students.zcu.cz
//

#include <stdexcept>
#include <vector>
#include "ParameterUtils.h"

constexpr unsigned int FTP_DATA_PORT = 20;
constexpr unsigned int FTP_CONTROL_PORT = 21;
constexpr unsigned int SSH_PORT = 22;
constexpr unsigned int SMTP_PORT = 25;
constexpr unsigned int DNS_PORT = 53;
constexpr unsigned int HTTP_PORT = 80;
constexpr unsigned int POP3_PORT = 110;
constexpr unsigned int SSL_PORT = 443;

namespace app {

    void validateCLIparams(int argc, char** params) {
        if (argc < 3) {
            throw std::invalid_argument("Invalid argument count!");
        }
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

    bool isIpAddress(std::string string) {
        std::vector<std::string> tokens;
        std::size_t pos;

        while ((pos = string.find('.')) != std::string::npos) {
            tokens.push_back(string.substr(0, pos));
            string.erase(0, pos + 1);
        }
        if (string.empty()) {
            return false;
        } else {
            tokens.push_back(string);
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
}

