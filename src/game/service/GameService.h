//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_GAMESERVICE_H
#define DAMA_SERVER_GAMESERVICE_H

#include <vector>
#include <array>
#include <list>
#include "../vo/Player.h"
#include "PlayerService.h"
#include "../vo/Game.h"
#include "../../socket/service/ConnectionService.h"

namespace app {

    /**
     * Service for manipulation with games.
     */
    class GameService {
    private: // private attributes
        app::ConnectionService m_connectionService;
    public: // public methods
        void createNewGame(app::Player *player);

        void removePlayerFromGame(int userId);

        std::list<app::Game*> findLobbyGames();

        Game *findWaitingGame(const std::string &playerNickname);

        void startNewGame(Game *game);

        app::Game* findGame(int userId);

        bool processTurn(int userId, int fromX, int fromY, int toX, int toY);

        void removeGame(int gameId);
    };
}



#endif //DAMA_SERVER_GAMESERVICE_H
