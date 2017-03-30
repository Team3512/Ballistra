// Copyright (c) FRC Team 3512, Spartatroniks 2014-2017. All Rights Reserved.

#include "LogStream.h"

LogStream::LogStream(Logger* logger) : std::ostream(new LogStreambuf(logger)) {
    m_logger = logger;
}

LogStream::~LogStream() { delete rdbuf(); }

void LogStream::setLevel(LogEvent::VerbosityLevel level) {
    static_cast<LogStreambuf*>(rdbuf())->setLevel(level);
}
