
#include "Logger.h"

#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

#define RESET       "\033[0m"
#define RED         "\033[31m"
#define BRIGHT_RED  "\033[91m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define CYAN        "\033[36m"

void MantleLogger::logMessage(LogLevel level, const std::string& message, const std::string& file, const std::string& function, u32 line) {

    std::string levelCode;
    std::string colorCode;

    switch (level) {
    case MG_LOG_LEVEL_DEBUG:
        levelCode = "DEBUG";
        colorCode = GREEN;
        break;
    case MG_LOG_LEVEL_INFO:
        levelCode = "INFO";
        colorCode = RESET;
        break;
    case MG_LOG_LEVEL_WARNING:
        levelCode = "WARNING";
        colorCode = YELLOW;
        break;
    case MG_LOG_LEVEL_ERROR:
        levelCode = "ERROR";
        colorCode = BRIGHT_RED;
        break;
    case MG_LOG_LEVEL_FATAL:
        levelCode = "FATAL";
        colorCode = RED;
        break;
    case MG_LOG_LEVEL_INTERNAL:
        levelCode = "INTERNAL";
        colorCode = CYAN;
        break;
    case MG_LOG_LEVEL_INTERNAL_ERROR:
        levelCode = "INTERNAL ERROR";
        colorCode = BRIGHT_RED;
        break;
    case MG_LOG_LEVEL_INTERNAL_FATAL:
        levelCode = "INTERNAL FATAL";
        colorCode = RED;
        break;
    default:
        levelCode = "UNKNOWN";
        colorCode = RESET;
        break;
    }

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
    std::tm* timeInfo = std::localtime(&currentTime);

    std::cout << std::put_time(timeInfo, "%Y-%m-%d, %H:%M") << " " << colorCode << levelCode << RESET << 
        " [" << file << " : " << line << "] " << function << "(): " << message << std::endl;
}