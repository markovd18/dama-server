//
// Author: markovd@students.zcu.cz
//

#include "GameRepository.h"

app::GameRepository &app::GameRepository::getInstance() {
    static GameRepository repository;
    return repository;
}

void app::GameRepository::save(app::Game &&game) {
    if (game.getId() != 0) {
        for (auto &m_game : m_games) {
            if (m_game.getId() == game.getId()) {
                m_game = game;
            }
        }
    } else {
        game.setId(m_availableId);
        m_games.emplace_back(game);
        m_availableId++;
    }
}

app::Game* app::GameRepository::findOneByUserId(const int userId) {
    for (auto &game : m_games) {
        if (((game.getPlayer1() != nullptr) && (game.getPlayer1()->getUserId() == userId)) ||
            ((game.getPlayer2() != nullptr) && (game.getPlayer2()->getUserId() == userId))) {
            return &game;
        }
    }

    return nullptr;
}

void app::GameRepository::remove(const int gameId) {
    for (int i = 0; i < m_games.size(); ++i) {
        if (m_games[i].getId() == gameId) {
            m_games.erase(m_games.begin() + i);
        }
    }
}

std::vector<app::Game*> app::GameRepository::findAllByState(const app::GameState state) {
    std::vector<app::Game*> games;
    for (auto &game : m_games) {
        if (game.getState() == state) {
            games.push_back(&game);
        }
    }

    return games;
}
