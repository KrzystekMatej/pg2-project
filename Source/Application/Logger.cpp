#include "Application/Logger.h"

std::mutex Logger::mutex_;

void Logger::log(LogLevel level, const std::string& message)
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::string levelStr = getLevelString(level);
    std::string logMessage = "[" + levelStr + "] " + message;

    std::cout << logMessage << "\n";

    std::ofstream logFile("log.txt", std::ios::app);
    if (logFile.is_open())
    {
        logFile << logMessage << "\n";
        logFile.close();
    }
}

std::string Logger::getLevelString(LogLevel level)
{
    switch (level)
    {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}
