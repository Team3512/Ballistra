// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

#include "LogStream.h"

LogStream::LogStream(Logger* logger) : std::ostream(new LogStreambuf(logger)) {
    m_logger = logger;
}

LogStream::~LogStream() { delete rdbuf(); }

void LogStream::setLevel(LogEvent::VerbosityLevel level) {
    static_cast<LogStreambuf*>(rdbuf())->setLevel(level);
}
