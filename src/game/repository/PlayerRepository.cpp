//
// Author: markovd@students.zcu.cz
//

#include <stdexcept>
#include "PlayerRepository.h"

app::PlayerRepository& app::PlayerRepository::getInstance() {
    static PlayerRepository repo;
    return repo;
}

app::Player app::PlayerRepository::findOne(const int userId) {
    for (const auto &player : m_players) {
        if (player.getUserId() == userId) {
            return player;
        }
    }

    throw std::invalid_argument(std::string("Player with userId ") + std::to_string(userId) + "not found");
}

void app::PlayerRepository::save(app::Player &&player) {
    m_players.emplace_back(std::move(player));
}

app::Player app::PlayerRepository::remove(const int userId) {
    for (int i = 0; i < m_players.size(); ++i) {
        if (m_players[i].getUserId() == userId) {
            app::Player removedPlayer(m_players[i]);
            m_players.erase(m_players.begin() + i);
            return removedPlayer;
        }
    }

    throw std::invalid_argument(std::string("Player with userId ") + std::to_string(userId) + "not found");
}
