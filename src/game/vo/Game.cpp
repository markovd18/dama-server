//
// Author: markovd@students.zcu.cz
//

#include "Game.h"

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
    player1Tokens.emplace_back(app::Token(0, 1));
    player1Tokens.emplace_back(app::Token(0, 3));
    player1Tokens.emplace_back(app::Token(0, 5));
    player1Tokens.emplace_back(app::Token(1, 0));
    player1Tokens.emplace_back(app::Token(1, 2));
    player1Tokens.emplace_back(app::Token(1, 4));

    player2Tokens.emplace_back(app::Token(4, 1));
    player2Tokens.emplace_back(app::Token(4, 3));
    player2Tokens.emplace_back(app::Token(4, 5));
    player2Tokens.emplace_back(app::Token(5, 0));
    player2Tokens.emplace_back(app::Token(5, 2));
    player2Tokens.emplace_back(app::Token(5, 4));

}

const std::vector<app::Token> *app::Game::getPlayer1Tokens() const {
    return &player1Tokens;
}

const std::vector<app::Token> *app::Game::getPlayer2Tokens() const {
    return &player2Tokens;
}
