
#ifndef MANTLE_LOGGER_HEADER_
#define MANTLE_LOGGER_HEADER_

#include "CustomTypes.h"

#include <time.h>
#include <string>

#define FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define LINE __LINE__
#define FUNC __func__

namespace MantleLogger {

    enum LogLevel {
        MG_LOG_LEVEL_FATAL,
        MG_LOG_LEVEL_ERROR,
        MG_LOG_LEVEL_WARNING,
        MG_LOG_LEVEL_INFO,
        MG_LOG_LEVEL_DEBUG,
        MG_LOG_LEVEL_INTERNAL,
        MG_LOG_LEVEL_INTERNAL_ERROR,
        MG_LOG_LEVEL_INTERNAL_FATAL
    };

    void logMessage(LogLevel level, const std::string& message, const std::string& file, const std::string& function, u32 line);
}

#endif // !MANTLE_LOGGER_HEADER_
