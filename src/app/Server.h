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
public:
    /**
     * Starts the entire application.
     *
     * @param command line parameters
     */
    static void run(const std::string& params);
};


#endif
