#ifndef LOGGER_H
#define LOGGER_H

#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

// Enum class for log levels
enum class LogLevel { TRACE, DEBUG, INFO, WARNING, ERROR, FATAL };

// Declare the LogLevelToString() function
std::string LogLevelToString(LogLevel level);

// Declare the GetCurrentTimeAsString() function
std::string GetCurrentTimeAsString();

// Log class that manages the log level and outputs log messages to the standard
// output stream
class Log {
public:
    // Constructor
    explicit Log(LogLevel level) : m_level(level) {}

    // Destructor that outputs the log message to the standard output stream
    ~Log() {
        // Create a string with the log message
        std::ostringstream oss;
        oss << "[" << LogLevelToString(m_level) << "]["
            << GetCurrentTimeAsString() << "]: " << m_oss.str();

        // Output the log message to the standard output stream
        std::cout << oss.str() << std::endl;
    }

    // Stream insertion operator that adds data to the log message
    template <typename T>
    Log& operator<<(const T& data) {
        m_oss << data;
        return *this;
    }

private:
    // The log level
    LogLevel m_level;

    // The stream to append log message data to
    std::ostringstream m_oss;
};

// Define the LOG() macro that creates an instance of the Log class with the
// specified log level
#define LOG(level) Log(LogLevel::level)

#endif  // LOGGER_H