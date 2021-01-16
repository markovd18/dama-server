//
// Author: markovd@students.zcu.cz
//

#include "GameService.h"
#include "../vo/Game.h"
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
            app::GameRepository::getInstance().remove(game->getId());
        }
    }
}

std::vector<app::Game*> app::GameService::findLobbyGames() {
    return app::GameRepository::getInstance().findAllByState(app::GameState::WAITING);
}
