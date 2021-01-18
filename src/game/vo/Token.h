//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_TOKEN_H
#define DAMA_SERVER_TOKEN_H

namespace app {

    /**
     * Class representing a game token, placed on the game board.
     */
    class Token {
    private: // private attributes
        int m_positionX;
        int m_positionY;
    public: // public methods
        Token(int positionX, int positionY);

        int getPositionX() const;

        void setPositionX(int positionX);

        int getPositionY() const;

        void setPositionY(int positionY);
    };
}


#endif //DAMA_SERVER_TOKEN_H
