//
// Author: markovd@students.zcu.cz
//

#include "Player.h"

app::Player::Player(int userId, std::string nickname) : m_userId(userId), m_nickname(std::move(nickname)), m_state{app::PlayerState::IN_LOBBY} {}

const std::string &app::Player::getNickname() const {
    return m_nickname;
}

int app::Player::getUserId() const {
    return m_userId;
}

app::PlayerState app::Player::getState() const {
    return m_state;
}

void app::Player::setState(app::PlayerState mState) {
    m_state = mState;
}
