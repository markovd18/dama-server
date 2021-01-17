//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_CONNECTIONSERVICE_H
#define DAMA_SERVER_CONNECTIONSERVICE_H

#include "../../game/service/PlayerService.h"
#include "../../request/Response.h"
#include "../vo/Connection.h"

namespace app {

    /**
     * Service for manipulation with connections.
     */
    class ConnectionService {
    private: // private attributes
        app::PlayerService m_playerService;
    public: // public methods
        void sendGameDeletedResponse(int userId);

        void sendResponse(const app::Response& response, int socket);

        void disconnect(int socket);

        bool processInvalidRequest(int socket);

        app::Connection *findConnection(int socket);

        app::Connection* findConnectionByUser(int userId);

        void sendGameCreatedResponse(const std::string& userNick);

        void addNewConnection(int socket);

    private: // private methods
        void sendMessage(const std::string& message, int socket);
    };
}


#endif //DAMA_SERVER_CONNECTIONSERVICE_H
