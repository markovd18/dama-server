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
        int m_userId = 0;
        PlayerState m_state;
    public: // public methods
        explicit Player(std::string nickname);

        [[nodiscard]] const std::string &getNickname() const;

        [[nodiscard]] int getUserId() const;

        [[nodiscard]] PlayerState getState() const;

        void setState(PlayerState mState);

        /**
         * Sets userId to given value. Note that this function should not be used anywhere in the program other than
         * in the player repository save method to set proper value.
         * @param userId value to set
         */
        void setUserId(int userId);
    };
}



#endif //DAMA_SERVER_PLAYER_H
