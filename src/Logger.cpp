// Logger.cpp

#include "Logger.h"

std::string LogLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE:
            return "TRACE";
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";

        // Add a default case that returns a string for any LogLevel value that
        // is not explicitly handled
        default:
            return "UNKNOWN";
    }
}

// Function to get the current time as a string in the format "YYYY-MM-DD
// HH:MM:SS.ssssss"
std::string GetCurrentTimeAsString() {
    // Get the current time
    auto now = std::chrono::system_clock::now();

    // Convert the time to a calendar time
    auto calendarTime = std::chrono::system_clock::to_time_t(now);

    // Create a stream to output the time to
    std::ostringstream oss;

    // Format the time as "YYYY-MM-DD HH:MM:SS.ssssss"
    oss << std::put_time(std::localtime(&calendarTime), "%Y-%m-%d %H:%M:%S");

    // Get the fractional seconds part of the time
    auto fractionalSeconds =
        std::chrono::duration_cast<std::chrono::microseconds>(
            now.time_since_epoch()) %
        1000000;

    // Append the fractional seconds to the time string
    oss << "." << std::setfill('0') << std::setw(6)
        << fractionalSeconds.count();

    // Return the time string
    return oss.str();
}
