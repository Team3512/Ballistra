// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

#pragma once

#include "LogStream.h"

/*!
  The SetLogLevel class provides a manipulator for
  describing the verbosity level of the message
  that follows. This manipulator can be used in
  place of the LogStream::setLevel function. One
  of the two must be called each time a stream is
  used to generate a log event.
 */
class SetLogLevel {
public:
    /*!
     The constructor.
     \param level The verbosity level to use for
     the next message.
     */
    SetLogLevel(LogEvent::VerbosityLevel level);

    friend LogStream& operator<<(LogStream& os, const SetLogLevel& in);

private:
    int m_level;
};
