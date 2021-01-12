//
// Author: markovd@students.zcu.cz
//

#include <fstream>
#include <ctime>
#include <filesystem>
#include "Logger.h"

namespace app {
    Logger::Logger() {
        std::filesystem::create_directory("logs");
        stream.open("logs/" + currentDate() + ".log", std::ios::app);
    }

    void Logger::error(const std::string &text) {
        log(text, "ERROR");
    }

    void Logger::warn(const std::string &text) {
        log(text, "WARN");
    }

    void Logger::info(const std::string &text) {
        log(text, "INFO");
    }

    void Logger::debug(const std::string &text) {
#ifndef NDEBUG
        log(text, "DEBUG");
#endif
    }

    void Logger::log(const std::string &text, const std::string &level) {
        std::string logText = currentTime() + " " + level + "\t" + text + '\n';
        stream << logText;
        stream.flush();
        std::cout << logText;
    }

    Logger &Logger::getInstance() {
        static Logger instance;
        return instance;
    }

    std::string currentTime() {
        time_t rawTime;
        struct tm *timeInfo;
        char buffer[80];

        time(&rawTime);
        timeInfo = localtime(&rawTime);

        strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", timeInfo);
        return std::string(buffer);
    }

    std::string currentDate() {
        time_t rawTime;
        struct tm *timeInfo;
        char buffer[80];

        time(&rawTime);
        timeInfo = localtime(&rawTime);

        strftime(buffer, 80, "%Y-%m-%d", timeInfo);
        return std::string(buffer);
    }
}