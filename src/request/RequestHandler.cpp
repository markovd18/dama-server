//
// Author: markovd@students.zcu.cz
//

#include <vector>
#include <string>
#include "RequestHandler.h"
#include "Response.h"
#include "../utils/ParameterUtils.h"
#include "../utils/Logger.h"
#include "../game/vo/Game.h"

app::Response app::RequestHandler::processRequest(const std::string &request) {
    std::vector<std::string> tokens(app::parseString(request, std::string{tokenDelimiter}));
    if (tokens.empty() || (tokens.size() < 2) || (!isNumber(tokens[0]))) {
        return app::Response(std::to_string(app::Response::GENERAL_ERROR) + '\n', false);
    }

    int userId = atoi(tokens[0].c_str());
    if (tokens[1] == getRequestTypeName(RequestType::CONNECT)) {
        /// Request to login
        if (tokens.size() == 3) {
            return processLoginRequest(userId, tokens[2]);
        }
    } else if (tokens[1] == getRequestTypeName(RequestType::LOGOUT)) {
        /// Request to logout
        if (tokens.size() == 2) {
            return processLogoutRequest(userId);
        }
    } else if (tokens[1] == getRequestTypeName(RequestType::CREATE_GAME)) {
        /// Request to create and join new game
        if (tokens.size() == 2) {
            return processCreateGameRequest(userId);
        }
    } else if (tokens[1] == getRequestTypeName(RequestType::EXIT_GAME)) {
        /// Request to exit the game
        if (tokens.size() == 2) {
            return processExitGameRequest(userId);
        }
    } else if (tokens[1] == getRequestTypeName(RequestType::GET_GAMES)) {
        /// Request to return all lobby games
        if (tokens.size() == 2) {
            return processGetGamesRequest(userId);
        }
    }

    return app::Response(std::to_string(app::Response::GENERAL_ERROR) + '\n', false);

}

std::string app::RequestHandler::getRequestTypeName(RequestType requestType) {
    return requestTypes[requestType];
}

app::Response app::RequestHandler::processLoginRequest(const int userId, const std::string& nickname) {
    if (!app::isValidNickname(nickname)) {
        return app::Response(std::to_string(app::Response::INVALID_NICKNAME) + '\n', false);
    }

    if (userId != 0) {
        return app::Response(std::to_string(app::Response::INVALID_USERID) + '\n', false);
    }

    app::Player* player = m_playerService.findPlayer(nickname);
    if (player != nullptr) { /// Player already exists, reconnect request
        if (player->getState() == app::PlayerState::DISCONNECTED) {
            return app::Response(std::to_string(app::Response::RECONNECT_OK) + '\n', true);
        } else {
            return app::Response(std::to_string(app::Response::CANNOT_RECONNECT) + '\n', false);
        }
    }

    /// Player doesn't exist, login request
    app::Player* newPlayer = m_playerService.addNewPlayer(nickname);
    return app::Response(std::to_string(app::Response::NEW_LOGIN_OK)
            + tokenDelimiter + std::to_string(newPlayer->getUserId()) + '\n', true);
}

app::Response app::RequestHandler::processLogoutRequest(const int userId) {
    app::Logger::getInstance().debug("Processing logout request for user ID: " + std::to_string(userId));
    app::Player* player = m_playerService.findPlayer(userId);

    if (player == nullptr) {
        app::Logger::getInstance().error("User with ID " + std::to_string(userId) + " not found!");
        return app::Response(std::to_string(app::Response::LOGOUT_INVALID_USER) + '\n', false);
    }
    if (player->getState() != app::PlayerState::IN_LOBBY) {
        app::Logger::getInstance().error("Only players in lobby can logout!");
        return app::Response(std::to_string(app::Response::CANNOT_LOGOUT) + '\n', false);
    }

    m_playerService.deletePlayer(userId);
    app::Logger::getInstance().debug("Player logged out.");
    return app::Response(std::to_string(app::Response::LOGOUT_OK) + '\n', true);
}

app::Response app::RequestHandler::processCreateGameRequest(const int userId) {
    app::Logger::getInstance().debug("Processing create new game request for user ID: " + std::to_string(userId));
    app::Player* player = m_playerService.findPlayer(userId);

    if (player == nullptr) {
        app::Logger::getInstance().error("User with ID " + std::to_string(userId) + " not found!");
        return app::Response(std::to_string(app::Response::CREATE_GAME_FAIL_ID) + '\n', false);
    }
    if (player->getState() != app::PlayerState::IN_LOBBY) {
        app::Logger::getInstance().error("User with ID " + std::to_string(userId) + " not in lobby!");
        return app::Response(std::to_string(app::Response::CREATE_GAME_FAIL_STATE) + '\n', false);
    }

    m_gameService.createNewGame(player);
    app::Logger::getInstance().debug("New game created!");
    return app::Response(std::to_string(app::Response::CREATE_GAME_OK) + '\n', true);
}

app::Response app::RequestHandler::processExitGameRequest(const int userId) {
    app::Logger::getInstance().debug("Processing exit game request for user ID: " + std::to_string(userId));
    app::Player* player = m_playerService.findPlayer(userId);

    if (player == nullptr) {
        app::Logger::getInstance().error("User with ID " + std::to_string(userId) + " not found!");
        return app::Response(std::to_string(app::Response::EXIT_GAME_FAIL_ID) + '\n', false);
    }
    app::PlayerState state = player->getState();
    if ((state != app::PlayerState::IN_GAME_T) && (state != app::PlayerState::IN_GAME_W)) {
        app::Logger::getInstance().error("User with ID " + std::to_string(userId) + " not in a game, cannot exit.");
        return app::Response(std::to_string(app::Response::EXIT_GAME_FAIL_STATE) + '\n', false);
    }

    m_gameService.removePlayerFromGame(userId);
    return app::Response(std::to_string(app::Response::EXIT_GAME_OK) + '\n', true);
}

app::Response app::RequestHandler::processGetGamesRequest(const int userId) {
    app::Player* player = m_playerService.findPlayer(userId);
    if (player == nullptr) {
        app::Logger::getInstance().error("User with ID " + std::to_string(userId) + " not found!");
        return app::Response(std::to_string(app::Response::GET_GAMES_FAIL) + '\n', false);
    }

    std::vector<app::Game*> lobbyGames = m_gameService.findLobbyGames();
    std::string response = std::to_string(app::Response::GET_GAMES_OK);
    for (auto &game : lobbyGames) {
        app::Player* waitingPlayer = game->getPlayer1();
        if (waitingPlayer == nullptr) {
            waitingPlayer = game->getPlayer2();
        }

        response += tokenDelimiter + waitingPlayer->getNickname();
    }
    response += '\n';

    return app::Response(response, true);
}
