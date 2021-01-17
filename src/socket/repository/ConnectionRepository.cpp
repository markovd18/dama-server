//
// Author: markovd@students.zcu.cz
//

#include <stdexcept>
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

app::Connection app::ConnectionRepository::remove(const int socket) {
    for (int i = 0; i < m_connections.size(); ++i) {
        if (m_connections[i].getSocket() == socket) {
            app::Connection connection(m_connections[i]);
            m_connections.erase(m_connections.begin() + i);
            return connection;
        }
    }

    throw std::invalid_argument("Connection from socket " + std::to_string(socket) + " not found!");
}

std::vector<app::Connection *> app::ConnectionRepository::findAllByPlayerState(app::PlayerState state) {
    std::vector<app::Connection*> connections;
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
