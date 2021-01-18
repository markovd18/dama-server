//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_CONNECTIONREPOSITORY_H
#define DAMA_SERVER_CONNECTIONREPOSITORY_H

#include <vector>
#include <list>
#include "../vo/Connection.h"
#include "../../game/vo/Player.h"

namespace app {

    /**
     * Repository class for individual connections. Class is a singleton, to keep the data set consistent
     * throughout every class that accesses it.
     */
    class ConnectionRepository {
    private: // private attributes
        std::list<app::Connection> m_connections;
    public: // public methods
        static ConnectionRepository& getInstance();
        app::Connection* findOne(int socket);
        app::Connection* findOneByUserId(int userId);
        void save(app::Connection&& connection);
        void remove(int socket);
        std::list<app::Connection*> findAllByPlayerState(app::PlayerState state);
    private: // private methods
        ConnectionRepository() = default;
    };
}


#endif //DAMA_SERVER_CONNECTIONREPOSITORY_H
