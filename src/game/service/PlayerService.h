//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_PLAYERSERVICE_H
#define DAMA_SERVER_PLAYERSERVICE_H

#include <string>
#include "../vo/Player.h"

namespace app {

    /**
     * Service for manipulation with players.
     */
    class PlayerService {
    public: // public methods

        /**
         * @brief Creates new player with given nickname and saves it into the repository.
         *
         * @param nickname new player's nickname
         * @return saved player
         *
         * Pointer to memory location of created player is returned back. DO NOT delete the pointer.
         * It would cause undefined behavior.
         */
        app::Player* addNewPlayer(const std::string& nickname);

        /**
         * @brief Finds player with given user ID.
         * @param userId user ID of a player
         * @return player with given user ID or nullptr
         *
         * If the player is not found, returns nullptr. DO NOT delete the pointer.
         * It would cause undefined behavior.
         */
        app::Player* findPlayer(int userId);

        /**
         * @brief Finds player with given nickname.
         * @param nickname nickname of a player
         * @return player with given nickname or nullptr
         *
         * If the player is not found, returns nullptr. DO NOT delete the pointer.
         * It would cause undefined behavior.
         */
        app::Player* findPlayer(const std::string& nickname);

        /**
         * @brief Deletes player with given ID.
         * @param userId id of player to delete
         * @throw invalid_argument if player with given user ID is not found
         *
         */
        app::Player deletePlayer(int userId);
    };
}



#endif //DAMA_SERVER_PLAYERSERVICE_H
