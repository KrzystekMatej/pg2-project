#include "Application/Logger.h"
#include <cassert>


int main()
{
    Logger::log(Logger::LogLevel::INFO, "This is an informational message.");
    Logger::log(Logger::LogLevel::WARNING, "This is a warning message.");
    Logger::log(Logger::LogLevel::ERROR, "This is an error message.");

    return 0;
}