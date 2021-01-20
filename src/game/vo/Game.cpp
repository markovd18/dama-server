//
// Author: markovd@students.zcu.cz
//

#include "Game.h"
#include "../../utils/ParameterUtils.h"

app::Game::Game(app::Player *player1, app::Player *player2) : m_player1(player1), m_player2(player2), m_state(GameState::WAITING) {
    if (m_player1 != nullptr) {
        m_player1->setState(app::PlayerState::IN_GAME_W);
    }
    if (m_player2 != nullptr) {
        m_player2->setState(app::PlayerState::IN_GAME_W);
    }
}

void app::Game::addPayer(app::Player *player) {
    if (player == nullptr) {
        return;
    }
    if (m_player1 == nullptr) {
        m_player1 = player;
    } else if (m_player2 == nullptr) {
        m_player2 = player;
    }
}

bool app::Game::start() {
    if (m_state == GameState::PLAYING || ((m_player1 == nullptr) || (m_player2 == nullptr))) {
        return false;
    }
    if (m_player1->getState() == app::PlayerState::DISCONNECTED ||
        m_player2->getState() == app::PlayerState::DISCONNECTED) {
        return false;
    }

    if (m_state == GameState::WAITING) {
        initializeTokens();
    }
    m_state = GameState::PLAYING;
    return true;
}

void app::Game::pause() {
    if (m_state == GameState::PLAYING) {
        m_state = GameState::PAUSED;
    }
}

app::Player *app::Game::getPlayer1() {
    return m_player1;
}

app::Player *app::Game::getPlayer2() {
    return m_player2;
}

bool app::Game::removePlayer(const int userId) {
    if (m_player1 != nullptr && m_player1->getUserId() == userId) {
        m_player1->setState(app::PlayerState::IN_LOBBY);
        m_player1 = nullptr;
        return true;
    } else if (m_player2 != nullptr && m_player2->getUserId() == userId) {
        m_player2->setState(app::PlayerState::IN_LOBBY);
        m_player2 = nullptr;
        return true;
    }

    return false;
}

bool app::Game::isEmpty() const {
    return ((m_player1 == nullptr) && (m_player2 == nullptr));
}

int app::Game::getId() const {
    return m_gameId;
}

app::GameState app::Game::getState() const {
    return m_state;
}

void app::Game::setId(int gameId) {
    if (m_gameId != 0) {
        return;
    }
    if (gameId > 0) {
        m_gameId = gameId;
    }
}

void app::Game::initializeTokens() {
    m_board[1][0] = m_player1->getUserId();
    m_board[3][0] = m_player1->getUserId();
    m_board[5][0] = m_player1->getUserId();
    m_board[0][1] = m_player1->getUserId();
    m_board[2][1] = m_player1->getUserId();
    m_board[4][1] = m_player1->getUserId();

    m_board[1][4] = m_player2->getUserId();
    m_board[3][4] = m_player2->getUserId();
    m_board[5][4] = m_player2->getUserId();
    m_board[0][5] = m_player2->getUserId();
    m_board[2][5] = m_player2->getUserId();
    m_board[4][5] = m_player2->getUserId();

}

std::vector<app::Token> app::Game::getPlayer1Tokens() const {
    std::vector<app::Token> tokens;
    for (int i = 0; i < ROW_COUNT; ++i) {
        for (int j = 0; j < ROW_COUNT; ++j) {
            if (m_board[i][j] == m_player1->getUserId()) {
                tokens.emplace_back(i, j);
            }
        }
    }

    return tokens;
}

std::vector<app::Token> app::Game::getPlayer2Tokens() const {
    std::vector<app::Token> tokens;
    for (int i = 0; i < ROW_COUNT; ++i) {
        for (int j = 0; j < ROW_COUNT; ++j) {
            if (m_board[i][j] == m_player2->getUserId()) {
                tokens.emplace_back(i, j);
            }
        }
    }

    return tokens;
}

bool app::Game::turn(const int userId, const int fromX, const int fromY, const int toX, const int toY) {
    /// Checking game and player states
    if (m_state != app::GameState::PLAYING) {
        return false;
    }
    if (m_player1 == nullptr || m_player2 == nullptr) {
        return false;
    }
    if (m_player1->getUserId() != userId && m_player2->getUserId() != userId) {
        return false;
    }
    app::Player* player = m_player1->getUserId() == userId ? m_player1 : m_player2;
    if (player->getState() != app::IN_GAME_T) {
        return false;
    }
    /// Checking bounds
    if (fromX < 0 || fromY < 0 || toX < 0 || toY < 0) {
        return false;
    }
    if (fromX > (ROW_COUNT - 1) || fromY > (ROW_COUNT - 1) || toX > (ROW_COUNT - 1) || toY > (ROW_COUNT - 1)) {
        return false;
    }
    if (m_board[fromX][fromY] != userId) {
        return false;
    }
    if (m_board[toX][toY] != 0) {
        return false;
    }

    bool draught = isDraught(fromX, fromY);
    /// Checking move backwards
    if (draught) {
        if (fromX == toX || fromY == toY) {
            return false;
        }
    } else {
        if (player == m_player1 && (fromX == toX || toY <= fromY)) {
            return false;
        }
        if (player == m_player2 && (fromX == toX || toY >= fromY)) {
            return false;
        }

    }
    /// Checking diagonal move, allowed by 1 and 2 only
    if (std::abs(fromX - toX) != std::abs(fromY - toY)) {
        return false;
    }
    if (!draught && (std::abs(fromX - toX) > 2 || std::abs(fromY - toY) > 2)) {
        return false;
    }

    app::Player* opponent = player == m_player1 ? m_player2 : m_player1;
    if (std::abs(fromX - toX) != 1) {
        /// Move by or more
        if (!draught) {
            if (!removeTokensBetweenPoints(
                    fromX, fromY, toX, toY, opponent == m_player1 ? getPlayer1Tokens() : getPlayer2Tokens())) {
                return false;
            }
        } else {
            removeTokensBetweenPoints(
                    fromX, fromY, toX, toY, opponent == m_player1 ? getPlayer1Tokens() : getPlayer2Tokens());
        }
    }

    m_board[fromX][fromY] = 0;
    m_board[toX][toY] = userId;
    if (draught) {
        moveDraught(fromX, fromY, toX, toY, player);
    }
    if (toY == 0 && !draught) {
        m_player2Draughts.emplace_back(app::Token(toX, toY));
    } else if (toY == (ROW_COUNT - 1) && !draught) {
        m_player1Draughts.emplace_back(app::Token(toX, toY));
    }
    player->setState(app::PlayerState::IN_GAME_W);
    opponent->setState(app::PlayerState::IN_GAME_T);
    /// Check if opponent has any tokens left
    if (player == m_player1 && getPlayer2Tokens().empty() ||
        player == m_player2 && getPlayer1Tokens().empty()) {
        m_isOver = true;
        m_player1->setState(app::PlayerState::IN_LOBBY);
        m_player2->setState(app::PlayerState::IN_LOBBY);
    }
    return true;
}

bool app::Game::isOver() const {
    return m_isOver;
}

bool app::Game::removeTokensBetweenPoints(const int fromX, const int fromY, const  int toX, const  int toY,
                                          const std::vector<app::Token>& tokens) {
    app::Token a(fromX, fromY);
    app::Token b(toX, toY);
    bool removedSomething = false;
    float epsilon = 0.001;

    for (const auto &c : tokens) {
        float distanceAB = app::distance(a.getPositionX(), a.getPositionY(), b.getPositionX(), b.getPositionY());
        float distanceAC = app::distance(a.getPositionX(), a.getPositionY(), c.getPositionX(), c.getPositionY());
        float distanceCB = app::distance(c.getPositionX(), c.getPositionY(), b.getPositionX(), b.getPositionY());
        if (std::abs(distanceAC + distanceCB - distanceAB) < epsilon) {
            m_board[c.getPositionX()][c.getPositionY()] = 0;
            removeDraught(c.getPositionX(), c.getPositionY());
            removedSomething = true;
        }
    }

    return removedSomething;
}

bool app::Game::isDraught(const int x, const int y) const {
    for (const auto &draught : m_player1Draughts) {
        if (draught.getPositionX() == x && draught.getPositionY() == y) {
            return true;
        }
    }

    for (const auto &draught : m_player2Draughts) {
        if (draught.getPositionX() == x && draught.getPositionY() == y) {
            return true;
        }
    }

    return false;
}

std::vector<app::Token> app::Game::getPlayer1Draughts() const {
    return m_player1Draughts;
}

std::vector<app::Token> app::Game::getPlayer2Draughts() const {
    return m_player2Draughts;
}

void app::Game::moveDraught(const int fromX, const int fromY, const int toX, const int toY, app::Player *player) {
    if (player == m_player1) {
        for (auto &draught : m_player1Draughts) {
            if (draught.getPositionX() == fromX && draught.getPositionY() == fromY) {
                draught.setPositionX(toX);
                draught.setPositionY(toY);
            }
        }
    } else {
        for (auto &draught : m_player2Draughts) {
            if (draught.getPositionX() == fromX && draught.getPositionY() == fromY) {
                draught.setPositionX(toX);
                draught.setPositionY(toY);
            }
        }
    }
}

void app::Game::removeDraught(int x, int y) {
    for (int i = 0; i < m_player1Draughts.size(); ++i) {
        if (m_player1Draughts[i].getPositionX() == x && m_player1Draughts[i].getPositionY() == y) {
            m_player1Draughts.erase(m_player1Draughts.begin() + i);
            return;
        }

    }
    for (int i = 0; i < m_player2Draughts.size(); ++i) {
        if (m_player2Draughts[i].getPositionX() == x && m_player2Draughts[i].getPositionY() == y) {
            m_player2Draughts.erase(m_player2Draughts.begin() + i);
            return;
        }

    }
}
