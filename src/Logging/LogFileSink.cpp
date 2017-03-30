// Copyright (c) FRC Team 3512, Spartatroniks 2014-2017. All Rights Reserved.

#include "LogFileSink.h"

LogFileSink::LogFileSink(std::string filename) {
    m_logfile.open(filename.c_str());
}

void LogFileSink::logEvent(LogEvent event) {
    m_logfile << event.toFormattedString();
    m_logfile.flush();
}
