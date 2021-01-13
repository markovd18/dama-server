//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_CONFIG_H
#define DAMA_SERVER_CONFIG_H

#include <string>

namespace app {

    /**
     * Configuration of the app.
     */
    struct config {
        /// Address to run on.
        std::string address;
        /// Port to listen on.
        unsigned int port;
        /// Max number of allowed connections at the same time.
        int maxConnections;
        /// Max number of games running at the same time.
        int maxGames;
    };
}
#endif //DAMA_SERVER_CONFIG_H
