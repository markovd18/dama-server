//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_GAMESERVICE_H
#define DAMA_SERVER_GAMESERVICE_H

#include <vector>
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
        app::PlayerService m_playerService;
        app::ConnectionService m_connectionService;
    public: // public methods
        void createNewGame(app::Player *player);

        void removePlayerFromGame(int userId);

        std::vector<app::Game*> findLobbyGames();
    };
}



#endif //DAMA_SERVER_GAMESERVICE_H
