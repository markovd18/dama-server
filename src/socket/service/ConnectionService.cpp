//
// Author: markovd@students.zcu.cz
//

#include <uv.h>
#include "ConnectionService.h"
#include "../repository/ConnectionRepository.h"
#include "../../utils/Logger.h"
#include "../../game/repository/GameRepository.h"

void app::ConnectionService::sendGameDeletedResponse(const int userId) {
    app::Player* player = m_playerService.findPlayer(userId);
    if (player != nullptr) {
        std::list<app::Connection*> lobbyConnections =
                app::ConnectionRepository::getInstance().findAllByPlayerState(app::PlayerState::IN_LOBBY);

        std::string message = std::to_string(app::Response::GAME_DELETED) + '|' + player->getNickname() + '\n';
        for (const auto & connection : lobbyConnections) {
            if (connection->getUserId() != userId) { /// Not sending to the player who exited the game
                sendMessage(message, connection->getSocket());
            }
        }
    }
}

void app::ConnectionService::sendGameCreatedResponse(const std::string& userNick) {
    std::list<app::Connection*> lobbyConnections =
            app::ConnectionRepository::getInstance().findAllByPlayerState(app::PlayerState::IN_LOBBY);

    std::string message = std::to_string(app::Response::NEW_GAME) + '|' + userNick + '\n';
    for (const auto &connection: lobbyConnections) {
        app::Player* player = m_playerService.findPlayer(userNick);
        if (player != nullptr && player->getNickname() == userNick) {
            sendMessage(message, connection->getSocket());
        }
    }
}

void app::ConnectionService::sendGameStartedResponse(app::Player *foundingPlayer) {
    if (foundingPlayer == nullptr) {
        return;
    }

    app::Connection* connection = app::ConnectionRepository::getInstance().findOneByUserId(foundingPlayer->getUserId());
    if (connection == nullptr) {
        return;
    }

    std::string message = std::to_string(app::Response::GAME_STARTED) + '\n';
    sendMessage(message, connection->getSocket());
}

void app::ConnectionService::sendOpponentLeftResponse(const int userId) {
    app::Player* player = m_playerService.findPlayer(userId);
    if (player != nullptr) {
        app::Connection* connection = findConnectionByUser(userId);
        if (connection != nullptr) {
            std::string message = std::to_string(app::Response::OPPONENT_LEFT) + '\n';
            sendMessage(message, connection->getSocket());
        }
    }
}

void app::ConnectionService::sendOpponentDisconnectedResponse(const int userId) {
    app::Player* player = m_playerService.findPlayer(userId);
    if (player != nullptr) {
        app::Connection* connection = findConnectionByUser(userId);
        std::string message = std::to_string(app::Response::OPPONENT_DC) + '\n';
        sendMessage(message, connection->getSocket());
    }
}

void app::ConnectionService::sendGameResumedResponse(const int userId) {
    app::Player* player = m_playerService.findPlayer(userId);
    if (player != nullptr) {
        app::Connection* connection = findConnectionByUser(userId);
        std::string message = std::to_string(app::Response::GAME_RESUMED) + '\n';
        sendMessage(message, connection->getSocket());
    }
}

void app::ConnectionService::sendResponse(const app::Response &response, int socket) {
    sendMessage(response.getMessage(), socket);
}

void app::ConnectionService::sendMessage(const std::string &message, int socket) {
    app::Logger::getInstance().debug(
            std::string("Sending message: ") + message + " through socket " + std::to_string(socket));
    if (send(socket, message.c_str(), message.size(), 0) < 0) {
        app::Logger::getInstance().error(
                "Error while sending message: " + message + "through socket " + std::to_string(socket));
    }
}

void app::ConnectionService::disconnect(const int socket) {
    app::Connection* connection = app::ConnectionRepository::getInstance().findOne(socket);
    if (connection == nullptr) {
        return;
    }

    if (connection->getUserId() != 0) {
        app::Player* player = m_playerService.findPlayer(connection->getUserId());
        if (player != nullptr) {
            app::Game* game = app::GameRepository::getInstance().findOneByUserId(player->getUserId());
            if (game != nullptr) {
                switch (game->getState()) {
                    case app::GameState::WAITING:
                        app::GameRepository::getInstance().remove(game->getId());
                        sendGameDeletedResponse(player->getUserId());
                        m_playerService.deletePlayer(player->getUserId());
                        break;
                    case app::GameState::PLAYING:
                        player->setState(app::PlayerState::DISCONNECTED);
                        game->pause();
                        /// Now opponent
                        player = game->getPlayer1()->getState() != app::PlayerState::DISCONNECTED
                                ? game->getPlayer1() : game->getPlayer2();
                        sendOpponentDisconnectedResponse(player->getUserId());
                        break;
                    case app::GameState::PAUSED:
                        /// Both players disconnected, deleting both players and the game too
                        m_playerService.deletePlayer(player->getUserId());
                        /// Opponent is the one with DC state
                        player = game->getPlayer1()->getState() == app::PlayerState::DISCONNECTED
                                ? game->getPlayer1() : game->getPlayer2();
                        m_playerService.deletePlayer(player->getUserId());
                        app::GameRepository::getInstance().remove(game->getId());
                        break;
                }
            } else {
                m_playerService.deletePlayer(player->getUserId());
            }
        }
    }

    sendResponse(app::Response(std::to_string(app::Response::CONNECTION_DROPPED) + '\n', true), socket);
    app::ConnectionRepository::getInstance().remove(socket);
}

bool app::ConnectionService::processInvalidRequest(const int socket) {
    app::Connection* connection = app::ConnectionRepository::getInstance().findOne(socket);
    if (connection != nullptr) {
        connection->incErrCount();
        if (connection->getErrorCount() == app::Connection::MAX_ERROR_COUNT) {
            disconnect(socket);
            return true;
        }
    }

    return false;
}

app::Connection *app::ConnectionService::findConnection(const int socket) {
    return app::ConnectionRepository::getInstance().findOne(socket);
}

app::Connection *app::ConnectionService::findConnectionByUser(const int userId) {
    return app::ConnectionRepository::getInstance().findOneByUserId(userId);
}

void app::ConnectionService::addNewConnection(const int socket) {
    app::ConnectionRepository::getInstance().save(app::Connection(socket));
}
