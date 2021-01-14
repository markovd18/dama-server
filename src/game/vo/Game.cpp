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

void app::Game::start() {
    if (m_state == GameState::PLAYING) {
        return;
    }

    m_state = GameState::PLAYING;
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
