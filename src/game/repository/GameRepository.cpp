//
// Author: markovd@students.zcu.cz
//

#include <stdexcept>
#include "GameRepository.h"

app::GameRepository &app::GameRepository::getInstance() {
    static GameRepository repository;
    return repository;
}

void app::GameRepository::save(app::Game game) {
    m_games.emplace_back(game);
}

app::Game app::GameRepository::findOneByUserId(const int userId) {
    for (auto &game : m_games) {
        if (game.getPlayer1() != nullptr && game.getPlayer1()->getUserId() == userId) {
            return game;
        } else if (game.getPlayer2() != nullptr && game.getPlayer2()->getUserId() == userId) {
            return game;
        }
    }

    throw std::invalid_argument("Game not found!");
}
