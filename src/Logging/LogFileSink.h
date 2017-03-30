// Copyright (c) FRC Team 3512, Spartatroniks 2014-2017. All Rights Reserved.

#pragma once

#include <fstream>
#include "LogSinkBase.h"

/*!
  A file sink for the logged events.
 */
class LogFileSink : public LogSinkBase {
public:
    LogFileSink(std::string filename);
    virtual ~LogFileSink() = default;

    /*!
      Write an event to the logfile.
      \param event The event to log.
     */
    void logEvent(LogEvent event);

private:
    std::ofstream m_logfile;
};
