//
// Created by Martin on 08.05.2020.
//

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <fstream>

namespace app {
    /**
	Class Logger is used to simplify logging of runtime information into a file.
    */
    class Logger {
    public:
        ~Logger() {
            stream.close();
        }

        void error(const std::string &text);

        void warn(const std::string &text);

        void info(const std::string &text);

        void debug(const std::string &text);

        static Logger &getInstance();

    private:
        Logger();

        void log(const std::string& text, const std::string& level);

        std::ofstream stream;
    };

    std::string currentTime();

    std::string currentDate();
}
#endif //LOGGER_H
