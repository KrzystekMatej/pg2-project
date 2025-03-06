#include "Logger.h"

std::mutex Logger::m_Mutex;

void Logger::log(LogLevel level, const std::string& message)
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    std::string levelStr = GetLevelString(level);
    std::string logMessage = "[" + levelStr + "] " + message;

    std::cout << logMessage << "\n";

    std::ofstream logFile("log.txt", std::ios::app);
    if (logFile.is_open())
    {
        logFile << logMessage << "\n";
        logFile.close();
    }
}

std::string Logger::GetLevelString(LogLevel level)
{
    switch (level)
    {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}
