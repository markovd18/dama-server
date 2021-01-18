//
// Author: markovd@students.zcu.cz
//

#include "ConnectionRepository.h"
#include "../../game/repository/PlayerRepository.h"

app::ConnectionRepository& app::ConnectionRepository::getInstance() {
    static ConnectionRepository repository;
    return repository;
}

void app::ConnectionRepository::save(app::Connection &&connection) {
    for (auto & m_connection : m_connections) {
        if (m_connection.getSocket() == connection.getSocket()) {
            m_connection = connection;
            break;
        }
    }

    m_connections.emplace_back(connection);
}

app::Connection *app::ConnectionRepository::findOne(const int socket) {
    for (auto &connection : m_connections) {
        if (connection.getSocket() == socket) {
            return &connection;
        }
    }

    return nullptr;
}

void app::ConnectionRepository::remove(const int socket) {
    auto it = m_connections.begin();
    while (it != m_connections.end()) {
        if (it->getSocket() == socket) {
            m_connections.erase(it);
            return;
        }
        it++;
    }
}

std::list<app::Connection *> app::ConnectionRepository::findAllByPlayerState(app::PlayerState state) {
    std::list<app::Connection*> connections;
    for (auto &connection : m_connections) {
        if (connection.getUserId() != 0) {
            app::Player* player = app::PlayerRepository::getInstance().findOne(connection.getUserId());
            if (player != nullptr && player->getState() == state) {
                connections.push_back(&connection);
            }
        }
    }

    return connections;
}

app::Connection *app::ConnectionRepository::findOneByUserId(const int userId) {
    for (auto &connection : m_connections) {
        if (connection.getUserId() == userId) {
            return &connection;
        }
    }

    return nullptr;
}
