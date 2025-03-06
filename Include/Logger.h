#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>

class Logger
{
public:
    enum class LogLevel
    {
        INFO,
        WARNING,
        ERROR
    };

    Logger() = delete;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    static void log(LogLevel level, const std::string& message);

private:
    static std::string GetLevelString(LogLevel level);
    static std::mutex m_Mutex;
};