//
// Created by Martin on 08.05.2020.
//

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <iostream>
#include <fstream>
#include <string>

namespace app {
    /**
	Class Logger is used to simplify logging of runtime information into a file.
    */
    class Logger {
    public:
        ~Logger() {
            m_stream.close();
        }

        void error(const std::string &text);

        void warn(const std::string &text);

        void info(const std::string &text);

        void debug(const std::string &text);

        static Logger &getInstance();

    private:
        Logger();

        void log(const std::string& text, const std::string& level);

        std::ofstream m_stream;
    };

    std::string currentTime();

    std::string currentDate();

    std::string dateTimeInFormat(const char* format);
}
#endif //LOGGER_H
