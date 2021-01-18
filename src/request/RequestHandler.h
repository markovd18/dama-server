//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_REQUESTHANDLER_H
#define DAMA_SERVER_REQUESTHANDLER_H

#include "Response.h"
#include "../game/service/PlayerService.h"
#include "../game/service/GameService.h"

namespace app {

    /**
     * Type of request, that can come from client.
     */
    enum RequestType {
        CONNECT = 0,
        LOGOUT = 1,
        CREATE_GAME = 2,
        GET_GAMES = 3,
        EXIT_GAME = 4,
        JOIN_GAME = 5,
        GET_GAME_STATE = 6
    };

    /**
    * Class for processing requests from clients connected to the server through BSD socket.
    */
     class RequestHandler {
     private:// private attributes
         static constexpr char tokenDelimiter = '|';
         static inline const std::string requestTypes[] = { "CONNECT", "LOGOUT", "CREATE_GAME", "GET_GAMES",
                                                            "EXIT_GAME", "JOIN_GAME", "GET_GAME_STATE"};

         app::PlayerService m_playerService;
         app::GameService m_gameService;
         app::ConnectionService m_connectionService;
     public: // public methods

         /**
          * @brief Checks if request message is valid, according to the application communication protocol and if so,
          * returns response, otherwise throws @a invalid_argument.
          *
          * @param request request to the server
          * @return response to the request
          * @throw invalid_argument if request is invalid (format-wise or logically)
          */
         app::Response processRequest(const std::string &request);

         /**
          * Returns string representation (name) of given request type.
          * @param requestType request type to turn into string
          * @return name of the request type
          */
         static std::string getRequestTypeName(RequestType requestType);

     private: // private methods
         app::Response processLoginRequest(int userId, const std::string& nickname);
         app::Response processLogoutRequest(int userId);

         app::Response processCreateGameRequest(int userId);

         app::Response processExitGameRequest(int userId);

         app::Response processGetGamesRequest(int userId);

         app::Response processJoinGameRequest(int userId, const std::string &opponentNick);

         app::Response processGetGameStateRequest(int userId);
     };
}


#endif //DAMA_SERVER_REQUESTHANDLER_H
