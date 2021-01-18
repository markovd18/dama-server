//
// Author: markovd@students.zcu.cz
//

#include <stdexcept>
#include "PlayerRepository.h"

app::PlayerRepository& app::PlayerRepository::getInstance() {
    static PlayerRepository repo;
    return repo;
}

app::Player* app::PlayerRepository::findOne(const int userId) {
    if (userId <= 0) {
        return nullptr;
    }
    for (auto &player : m_players) {
        if (player.getUserId() == userId) {
            return &player;
        }
    }

    return nullptr;
}

app::Player *app::PlayerRepository::findOne(const std::string &nickname) {
    for (auto &player : m_players) {
        if (player.getNickname() == nickname) {
            return &player;
        }
    }

    return nullptr;
}

void app::PlayerRepository::save(app::Player&& player) {
    if (player.getUserId() != 0) {
        for (auto &m_player : m_players) {
            if (m_player.getUserId() == player.getUserId()) {
                m_player = player;
            }
        }
    } else {
        player.setUserId(m_availableId);
        m_players.emplace_back(std::move(player));
        m_availableId++;
    }
}

void app::PlayerRepository::remove(const int userId) {
    auto it = m_players.begin();
    while (it != m_players.end()) {
        if (it->getUserId() == userId) {
            m_players.erase(it);
            break;
        }
        it++;
    }
}

int app::PlayerRepository::getPlayerCount() const {
    return m_players.size();
}
