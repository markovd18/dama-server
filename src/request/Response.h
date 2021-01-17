//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_RESPONSE_H
#define DAMA_SERVER_RESPONSE_H

#include <string>
#include <utility>

namespace app {

    /**
     * Class representing a response to a request from client.
     */
    class Response {
    public: // public attributes
        static constexpr int GENERAL_ERROR = 450;
        static constexpr int NEW_LOGIN_OK = 200;
        static constexpr int RECONNECT_OK = 201;
        static constexpr int LOGOUT_OK = 202;
        static constexpr int GET_GAMES_OK = 203;
        static constexpr int CREATE_GAME_OK = 204;
        static constexpr int EXIT_GAME_OK = 205;
        static constexpr int INVALID_NICKNAME = 400;
        static constexpr int INVALID_USERID = 401;
        static constexpr int CANNOT_RECONNECT = 402;
        static constexpr int LOGOUT_INVALID_USER = 403;
        static constexpr int CANNOT_LOGOUT = 404;
        static constexpr int GET_GAMES_FAIL = 405;
        static constexpr int CREATE_GAME_FAIL_STATE = 406;
        static constexpr int CREATE_GAME_FAIL_ID = 407;
        static constexpr int EXIT_GAME_FAIL_ID = 408;
        static constexpr int EXIT_GAME_FAIL_STATE = 409;

        static constexpr int NEW_GAME = 250;
        static constexpr int GAME_DELETED = 260;
    private: // private attributes
        bool m_wasValid;
        std::string m_message;
    public: // public methods
        Response(std::string  messsage, const bool wasRequestValid)
                            : m_message(std::move(messsage)), m_wasValid(wasRequestValid) {
        }

        [[nodiscard]] bool wasRequestValid() const {
            return m_wasValid;
        }

        [[nodiscard]] const std::string &getMessage() const {
            return m_message;
        }
    };
}


#endif //DAMA_SERVER_RESPONSE_H
