//
// Author: markovd@students.zcu.cz
//

#include "Token.h"

app::Token::Token(int positionX, int positionY)
                            : m_positionX(positionX >= 0 ? positionX : -1),
                            m_positionY(positionY >= 0 ? positionY : -1) {
    //
}

int app::Token::getPositionX() const {
    return m_positionX;
}

void app::Token::setPositionX(const int positionX) {
    m_positionX = positionX;
}

int app::Token::getPositionY() const {
    return m_positionY;
}

void app::Token::setPositionY(const int positionY) {
    m_positionY = positionY;
}
