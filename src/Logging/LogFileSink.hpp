// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

#pragma once

#include <fstream>
#include <string>

#include "LogSinkBase.hpp"

/*!
  A file sink for the logged events.
 */
class LogFileSink : public LogSinkBase {
public:
    explicit LogFileSink(std::string filename);
    virtual ~LogFileSink() = default;

    /*!
      Write an event to the logfile.
      \param event The event to log.
     */
    void logEvent(LogEvent event);

private:
    std::ofstream m_logfile;
};