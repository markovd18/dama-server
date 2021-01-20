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
        static constexpr int NEW_LOGIN_OK = 200;
        static constexpr int RECONNECT_OK = 201;
        static constexpr int LOGOUT_OK = 202;
        static constexpr int GET_GAMES_OK = 203;
        static constexpr int CREATE_GAME_OK = 204;
        static constexpr int EXIT_GAME_OK = 205;
        static constexpr int JOIN_GAME_OK = 206;
        static constexpr int GET_GAME_STATE_OK = 207;
        static constexpr int TURN_OK = 208;

        static constexpr int INVALID_NICKNAME = 400;
        static constexpr int CANNOT_RECONNECT = 401;
        static constexpr int JOIN_GAME_FAIL_NICK = 402;
        static constexpr int INVALID_TURN = 403;

        static constexpr int GAME_OVER = 210;
        static constexpr int NEW_GAME = 250;
        static constexpr int GAME_DELETED = 260;
        static constexpr int GAME_STARTED = 270;
        static constexpr int OPPONENT_LEFT = 280;
        static constexpr int OPPONENT_DC = 290;
        static constexpr int GAME_RESUMED = 295;

        static constexpr int INVALID_ID = 410;
        static constexpr int INVALID_STATE = 420;
        static constexpr int GENERAL_ERROR = 450;
        static constexpr int CONNECTION_DROPPED = 460;
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
