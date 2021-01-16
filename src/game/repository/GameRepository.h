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
        int m_availableId = 1;
    public: // public methods
        static GameRepository& getInstance();

        /**
         * Saves given game to the repository. Note, that any game, with ID higher than 0, will be checked
         * to be just updated. If no game with the same ID is not found, the instance will NOT be saved. Only
         * game instances with ID = 0 will be made into new entry.
         *
         * @param game game to save
         */
        void save(app::Game&& game);
        app::Game* findOneByUserId(int userId);

        void remove(int gameId);

        std::vector<app::Game*> findAllByState(GameState state);

    private: // private methods
        GameRepository() = default;
    };
}


#endif //DAMA_SERVER_GAMEREPOSITORY_H
