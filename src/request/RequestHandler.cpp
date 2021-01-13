//
// Author: markovd@students.zcu.cz
//

#include <vector>
#include <string>
#include <stdexcept>
#include "RequestHandler.h"
#include "Response.h"
#include "../utils/ParameterUtils.h"

app::Response app::RequestHandler::processRequest(const std::string &request) {
    std::vector<std::string> tokens(app::parseString(request, std::string{tokenDelimiter}));
    if (tokens.empty()) {
        return app::Response(std::to_string(app::Response::GENERAL_ERROR), false);
    }
    if (tokens.size() < 2) {
        return app::Response(std::to_string(app::Response::GENERAL_ERROR), false);
    }
    if (!isNumber(tokens[0])){
        return app::Response(std::to_string(app::Response::GENERAL_ERROR), false);
    }

    if (tokens[1] == getRequestTypeName(RequestType::CONNECT)) {
        if (tokens.size() < 3) {
            throw std::invalid_argument("User nickname is missing!");
        }
        return processLoginRequest(atoi(tokens[0].c_str()), tokens[2]);
    } else if (tokens[1] == getRequestTypeName(RequestType::TURN)) {
        //TODO markoda process turn request
    }

    return app::Response("", false);

}

std::string app::RequestHandler::getRequestTypeName(RequestType requestType) {
    return requestTypes[requestType];
}

app::Response app::RequestHandler::processLoginRequest(const int userId, const std::string& nickname) {
    //TODO markovda access to all logged in players and games
    if (!app::isValidNickname(nickname)) {
        return app::Response(std::to_string(app::Response::INVALID_NICKNAME), false);
    }
    if (userId == 0) {

    }

    return app::Response("", false);
}
