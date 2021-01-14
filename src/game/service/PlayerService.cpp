//
// Author: markovd@students.zcu.cz
//

#include "PlayerService.h"
#include "../repository/PlayerRepository.h"
#include "../../utils/Logger.h"

app::Player* app::PlayerService::addNewPlayer(const std::string &nickname) {
    int userId = app::PlayerRepository::getInstance().getHighestId() + 1;

    app::PlayerRepository::getInstance().save(app::Player(userId, nickname));

    app::Logger::getInstance().debug("Added new player: ID=" + std::to_string(userId) + ", Nickname=" + nickname);
    return app::PlayerRepository::getInstance().findOne(userId);
}

app::Player* app::PlayerService::findPlayer(const int userId) {
    app::Logger::getInstance().debug("Searching for player with ID: " + std::to_string(userId));
    return app::PlayerRepository::getInstance().findOne(userId);
}

app::Player *app::PlayerService::findPlayer(const std::string &nickname) {
    app::Logger::getInstance().debug("Searching for player with nickname: " + nickname);
    return app::PlayerRepository::getInstance().findOne(nickname);
}

app::Player app::PlayerService::deletePlayer(const int userId) {
    app::Logger::getInstance().debug("Deleting player with user ID: " + std::to_string(userId));
    return app::PlayerRepository::getInstance().remove(userId);
}
