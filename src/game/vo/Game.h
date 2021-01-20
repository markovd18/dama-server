//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_GAME_H
#define DAMA_SERVER_GAME_H

#include <vector>
#include "Player.h"
#include "Token.h"

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
    public: // public attributer
        static constexpr int ROW_COUNT = 6;
    private: // private attributes
        int m_gameId = 0;
        app::Player* m_player1;
        app::Player* m_player2;
        app::GameState m_state;
        std::vector<app::Token> m_player1Draughts;
        std::vector<app::Token> m_player2Draughts;
        int m_board[ROW_COUNT][ROW_COUNT] { 0 };
        bool m_isOver = false;
    public: // public methods
        Game(app::Player* player1, app::Player* player2);
        app::Player* getPlayer1();
        app::Player* getPlayer2();
        void addPayer(app::Player* player);
        bool start();
        void pause();
        bool removePlayer(int userId);
        [[nodiscard]] bool isEmpty() const;
        [[nodiscard]] int getId() const;
        [[nodiscard]] GameState getState() const;
        [[nodiscard]] std::vector<app::Token> getPlayer1Tokens() const;
        [[nodiscard]] std::vector<app::Token> getPlayer2Tokens() const;
        [[nodiscard]] std::vector<app::Token> getPlayer1Draughts() const;
        [[nodiscard]] std::vector<app::Token> getPlayer2Draughts() const;
        [[nodiscard]] bool isOver() const;
        bool turn(int userId, int fromX, int fromY, int toX, int toY);

        /**
         * This function sets ID of the game to given value. Note, that this method should not be used anywhere
         * in the program, but in the game repository, to set it proper value.
         */
        void setId(int gameId);

    private: // private methods
        void initializeTokens();
        bool removeTokensBetweenPoints(int fromX, int fromY, int toX, int toY, const std::vector<app::Token>& tokens);
        bool isDraught(int x, int y) const;

        void moveDraught(const int fromX, const int fromY, const int toX, const int toY, Player *player);

        void removeDraught(int x, int y);
    };
}


#endif //DAMA_SERVER_GAME_H
