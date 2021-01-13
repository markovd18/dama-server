//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_PLAYER_H
#define DAMA_SERVER_PLAYER_H

#include <string>

namespace app {

    enum PlayerState {
        /// Player is in lobby, waiting to get into the game.
        IN_LOBBY = 0,
        /// Player is in the game and it is his turn to play.
        IN_GAME_T = 1,
        /// Player is in the game and is waiting for opponent to play or to connect.
        IN_GAME_W = 2,
        /// Player has unexpectedly disconnected.
        DISCONNECTED = 3
    };

    /**
     * Class representing player in the draughts game.
     */
    class Player {
    private: // private attributes
        /// Players nickname
        std::string m_nickname;
        /// Players defining user ID
        int m_userId;
        PlayerState m_state;
    public: // public methods
        Player(int userId, std::string nickname);

        [[nodiscard]] const std::string &getNickname() const;

        [[nodiscard]] int getUserId() const;

        [[nodiscard]] PlayerState getState() const;

        void setState(PlayerState mState);
    };
}



#endif //DAMA_SERVER_PLAYER_H
