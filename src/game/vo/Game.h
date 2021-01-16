//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_GAME_H
#define DAMA_SERVER_GAME_H

#include "Player.h"

namespace app {

    /**
     * Individual states of the game.
     */
    enum GameState {
        /// Waiting for second player to join.
        WAITING = 0,
        /// Game is playing.
        PLAYING = 1,
        /// Game is paused, waiting for player to reconnect.
        PAUSED = 2
    };

    /**
     * Class representing individual games.
     */
    class Game {
    private: // private attributes
        int m_gameId = 0;
        app::Player* m_player1;
        app::Player* m_player2;
        app::GameState m_state;
    public: // public methods
        Game(app::Player* player1, app::Player* player2);
        app::Player* getPlayer1();
        app::Player* getPlayer2();
        void addPayer(app::Player* player);
        void start();
        void pause();
        bool removePlayer(int userId);
        [[nodiscard]] bool isEmpty() const;
        [[nodiscard]] int getId() const;
        [[nodiscard]] GameState getState() const;

        /**
         * This function sets ID of the game to given value. Note, that this method should not be used anywhere
         * in the program, but in the game repository, to set it proper value.
         */
        void setId(int gameId);
    };
}


#endif //DAMA_SERVER_GAME_H
