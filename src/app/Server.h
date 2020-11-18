//
// Author markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_SERVER_H
#define DAMA_SERVER_SERVER_H

#include <string>

/**
 * Static class that represents the application. Handles the input parameters from commandline
 */
class Server {
private://private attributes
    /**
     * Default port that the server is listening on
     */
    static constexpr int PORT = 8080;
public:
    /**
     * Starts the entire application.
     *
     * @param command line parameters
     */
    static void run();
};


#endif
