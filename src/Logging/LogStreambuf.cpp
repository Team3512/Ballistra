// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

#include "LogStreambuf.hpp"

LogStreambuf::LogStreambuf(Logger* logger) { m_logger = logger; }

void LogStreambuf::setLevel(LogEvent::VerbosityLevel level) { m_level = level; }

int LogStreambuf::sync() {
    if (m_level != LogEvent::VERBOSE_NONE)
        m_logger->logEvent(LogEvent(m_buf, m_level));

    m_buf = "";
    m_level = LogEvent::VERBOSE_NONE;

    return 0;
}

std::streamsize LogStreambuf::xsputn(const char* s, std::streamsize n) {
    m_buf += std::string(s, n);

    return n;
}
