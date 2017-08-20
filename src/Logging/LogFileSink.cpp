// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

#include "LogFileSink.hpp"

LogFileSink::LogFileSink(std::string filename) {
    m_logfile.open(filename.c_str());
}

void LogFileSink::logEvent(LogEvent event) {
    m_logfile << event.toFormattedString();
    m_logfile.flush();
}
