//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_PLAYERREPOSITORY_H
#define DAMA_SERVER_PLAYERREPOSITORY_H

#include <vector>
#include "../vo/Player.h"

namespace app {

    /**
     * Repository class for individual players. Class is a singleton, to keep the data set consistent
     * throughout every class that accesses it.
     */
    class PlayerRepository {
    private: // private attributes
        std::vector<app::Player> m_players;

    public: // public methods
        static PlayerRepository& getInstance();

        app::Player findOne(int userId);
        void save(app::Player&& player);
        app::Player remove(int userId);
    private: // private methods
        PlayerRepository() = default;
    };
}


#endif //DAMA_SERVER_PLAYERREPOSITORY_H
