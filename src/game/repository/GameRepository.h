//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_GAMEREPOSITORY_H
#define DAMA_SERVER_GAMEREPOSITORY_H

#include <vector>
#include "../vo/Game.h"

namespace app {

    /**
     * Repository class for individual games. Class is a singleton, to keep the data set consistent
     * throughout every class that accesses it.
     */
    class GameRepository {
    private: // private attributes
        std::vector<app::Game> m_games;
    public: // public methods
        static GameRepository& getInstance();

        void save(app::Game game);
        app::Game findOneByUserId(int userId);

    private: // private methods
        GameRepository() = default;
    };
}


#endif //DAMA_SERVER_GAMEREPOSITORY_H
