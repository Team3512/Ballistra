// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

#include "Logger.hpp"

Logger::Logger() { resetInitialTime(); }

void Logger::logEvent(LogEvent event) {
    event.setInitialTime(m_initialTime);

    for (auto& sink : m_sinkList) {
        if (sink->testVerbosityLevel(event.getVerbosityLevel())) {
            sink->logEvent(event);
        }
    }
}

void Logger::addLogSink(LogSinkBase* sink) { m_sinkList.push_back(sink); }

void Logger::removeLogSink(LogSinkBase* sink) { m_sinkList.remove(sink); }

Logger::LogSinkBaseList Logger::listLogSinks() { return m_sinkList; }

void Logger::resetInitialTime() { m_initialTime = std::time(nullptr); }

void Logger::setInitialTime(std::time_t time) { m_initialTime = time; }
