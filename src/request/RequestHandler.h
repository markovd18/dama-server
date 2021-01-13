//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_REQUESTHANDLER_H
#define DAMA_SERVER_REQUESTHANDLER_H

#include "Response.h"
#include "../game/service/PlayerService.h"

namespace app {

    /**
     * Type of request, that can come from client.
     */
    enum RequestType {
        CONNECT = 0,
        TURN = 1
    };

    /**
    * Class for processing requests from clients connected to the server through BSD socket.
    */
     class RequestHandler {
     private:// private attributes
         static constexpr char tokenDelimiter = '|';
         static inline const std::string requestTypes[] = { "CONNECT", "TURN" };

         app::PlayerService playerService;
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
          * @param requestType
          * @return
          */
         static std::string getRequestTypeName(RequestType requestType);

     private: // private methods
         app::Response processLoginRequest(int userId, const std::string& nickname);
     };
}


#endif //DAMA_SERVER_REQUESTHANDLER_H
