// Copyright (c) FRC Team 3512, Spartatroniks 2014-2017. All Rights Reserved.

#include "LogSinkBase.h"

void LogSinkBase::setVerbosityLevels(LogEvent::VerbosityLevel levels) {
    m_verbosity = levels;
}

LogEvent::VerbosityLevel LogSinkBase::getVerbosityLevels() {
    return m_verbosity;
}

void LogSinkBase::enableVerbosityLevels(LogEvent::VerbosityLevel levels) {
    m_verbosity |= levels;
}

void LogSinkBase::disableVerbosityLevels(LogEvent::VerbosityLevel levels) {
    m_verbosity &= ~(levels);
}

bool LogSinkBase::testVerbosityLevel(LogEvent::VerbosityLevel levels) {
    return m_verbosity & levels;
}
