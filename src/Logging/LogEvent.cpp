// Copyright (c) FRC Team 3512, Spartatroniks 2014-2017. All Rights Reserved.

#include "LogEvent.h"

#include <iomanip>
#include <sstream>

LogEvent::LogEvent(std::string data, VerbosityLevel level) {
    m_level = level;
    m_timestamp = std::time(nullptr);
    m_buffer = data;
    m_initialTime = 0;
}

LogEvent::LogEvent(std::string data, VerbosityLevel level,
                   std::time_t timestamp) {
    m_level = level;
    m_timestamp = timestamp;
    m_buffer = data;
    m_initialTime = 0;
}

LogEvent::VerbosityLevel LogEvent::getVerbosityLevel() { return m_level; }

time_t LogEvent::getAbsoluteTimestamp() { return m_timestamp; }

time_t LogEvent::getRelativeTimestamp() {
    if (m_initialTime == 0) return 0;

    return m_timestamp - m_initialTime;
}

std::string LogEvent::getData() { return m_buffer; }

std::string LogEvent::toFormattedString() {
    std::stringstream ss;

    ss << std::left << "[" << std::setw(8) << getRelativeTimestamp() << " "
       << verbosityLevelChar(getVerbosityLevel()) << "] " << getData() << "\n";

    return ss.str();
}

std::string LogEvent::verbosityLevelString(VerbosityLevel levels) {
    std::string level_str;

    if (levels & VERBOSE_ERROR) level_str += "ERROR ";

    if (levels & VERBOSE_WARN) level_str += "WARN ";

    if (levels & VERBOSE_INFO) level_str += "INFO ";

    if (levels & VERBOSE_DEBUG) level_str += "DEBUG ";

    if (levels & VERBOSE_USER) level_str += "USER ";

    return level_str;
}

char LogEvent::verbosityLevelChar(VerbosityLevel level) {
    switch (level) {
        case VERBOSE_ERROR:
            return 'E';
        case VERBOSE_WARN:
            return 'W';
        case VERBOSE_INFO:
            return 'I';
        case VERBOSE_DEBUG:
            return 'D';
        case VERBOSE_USER:
            return 'U';
        case VERBOSE_NONE:
        case VERBOSE_ALL:
        default:
            return 'X';
    }
}

void LogEvent::setInitialTime(std::time_t initial) { m_initialTime = initial; }
