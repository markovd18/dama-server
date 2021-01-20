//
// Author: markovd@students.zcu.cz
//

#include "GameService.h"
#include "../repository/GameRepository.h"

void app::GameService::createNewGame(app::Player *player) {
    if (player == nullptr) {
        return;
    }

    app::GameRepository::getInstance().save(app::Game(player, nullptr));
}

void app::GameService::removePlayerFromGame(const int userId) {
    app::Game* game = app::GameRepository::getInstance().findOneByUserId(userId);
    if (game != nullptr) {
        game->removePlayer(userId);
        if (game->isEmpty()) {
            if (game->getState() == app::GameState::WAITING) {
                /// In lobby, send info about game deletion
                m_connectionService.sendGameDeletedResponse(userId);
            }
        } else {
            /// Sending info to the remaining player that the opponent left and removing him from the game
            if (game->getPlayer1() != nullptr) {
                m_connectionService.sendOpponentLeftResponse(game->getPlayer1()->getUserId());
                game->removePlayer(game->getPlayer1()->getUserId());
            } else {
                m_connectionService.sendOpponentLeftResponse(game->getPlayer2()->getUserId());
                game->removePlayer(game->getPlayer2()->getUserId());
            }
        }
        /// Deleting the game either way in the end
        app::GameRepository::getInstance().remove(game->getId());
    }
}

std::list<app::Game*> app::GameService::findLobbyGames() {
    return app::GameRepository::getInstance().findAllByState(app::GameState::WAITING);
}

app::Game *app::GameService::findWaitingGame(const std::string &playerNickname) {
    return app::GameRepository::getInstance().findOneByStateAndNickname(app::GameState::WAITING, playerNickname);
}

void app::GameService::startNewGame(app::Game *game) {
    if (game == nullptr) {
        return;
    }

    if (game->start()) {
        game->getPlayer1()->setState(app::PlayerState::IN_GAME_T);
        game->getPlayer2()->setState(app::PlayerState::IN_GAME_W);
        m_connectionService.sendGameStartedResponse(game->getPlayer1());
    }
}

app::Game *app::GameService::findGame(const int userId) {
    return app::GameRepository::getInstance().findOneByUserId(userId);
}

bool app::GameService::processTurn(const int userId, const int fromX, const int fromY, const int toX, const int toY) {
    app::Game* game = app::GameRepository::getInstance().findOneByUserId(userId);
    if (game == nullptr) {
        return false;
    }

    return game->turn(userId, fromX, fromY, toX, toY);
}

void app::GameService::removeGame(const int gameId) {
    app::GameRepository::getInstance().remove(gameId);
}
