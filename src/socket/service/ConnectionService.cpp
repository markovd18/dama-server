//
// Author: markovd@students.zcu.cz
//

#include <uv.h>
#include "ConnectionService.h"
#include "../repository/ConnectionRepository.h"
#include "../../utils/Logger.h"

void app::ConnectionService::sendGameDeletedResponse(const int userId) {
    app::Player* player = m_playerService.findPlayer(userId);
    if (player != nullptr) {
        std::vector<app::Connection*> lobbyConnections =
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
    std::vector<app::Connection*> lobbyConnections =
            app::ConnectionRepository::getInstance().findAllByPlayerState(app::PlayerState::IN_LOBBY);

    std::string message = std::to_string(app::Response::NEW_GAME) + '|' + userNick + '\n';
    for (const auto &connection: lobbyConnections) {
        app::Player* player = m_playerService.findPlayer(userNick);
        if (player != nullptr && player->getNickname() != userNick) {
            sendMessage(message, connection->getSocket());
        }
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
    if (connection != nullptr) {
        if (connection->getUserId() != 0) {
            //TODO markovda delete player or set state disconnected
        }

        app::ConnectionRepository::getInstance().remove(socket);

    }
}

bool app::ConnectionService::processInvalidRequest(const int socket) {
    app::Connection* connection = app::ConnectionRepository::getInstance().findOne(socket);
    if (connection != nullptr) {
        connection->incErrCount();
        if (connection->getErrorCount() == app::Connection::MAX_ERROR_COUNT) {
            /*app::Player* player = m_playerService.findPlayer(connection->getUserId());
            if (player != nullptr) {
                if (player->getState() != app::PlayerState::IN_LOBBY) {
                    player->setState(app::PlayerState::DISCONNECTED);
                }
            }*/ //TODO markovda disconnect player, set state of game and player
            app::ConnectionRepository::getInstance().remove(socket);
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
