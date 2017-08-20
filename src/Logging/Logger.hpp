// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

#pragma once

#include <list>
#include <string>

#include "LogEvent.hpp"
#include "LogSinkBase.hpp"

/**
 * A logging engine.
 */
class Logger {
public:
    typedef std::list<LogSinkBase*> LogSinkBaseList;

    /*!
      The constructor.
      Calls resetInitialTime()
     */
    Logger();
    virtual ~Logger() = default;

    /*!
      Logs an event to all registered LogSinkBase sinks
      whose verbosity levels contain that of the event.
      (See LogSinkBase::TestVerbosityLevel)
      \param event The event to log
     */
    void logEvent(LogEvent event);

    /*!
      Registers a sink for log events with the logging
      engine. All events with a verbosity level matching
      that of the sink (see LogSinkBase::TestVerbosityLevel)
      which are logged after the sink is registered will
      be sent to the sink.
      \param sink The sink class instance to register
     */
    void addLogSink(LogSinkBase* sink);

    /*!
      De-registers a sink from the logging engine.
      \param sink The sink to de-register
     */
    void removeLogSink(LogSinkBase* sink);

    /*!
      Returns a list of currently registered sinks.
      \return A list of currently registered sinks.
     */
    LogSinkBaseList listLogSinks();

    /*!
      Sets the 'initial' time to the current time.
      This initial time is used to calculate an
      event's relative time.
     */
    void resetInitialTime();

    /*!
      Sets the 'initial' time to the specified time.
      This initial time is used to calculate an
      event's relative time.
     */
    void setInitialTime(std::time_t time);

private:
    LogSinkBaseList m_sinkList;
    std::time_t m_initialTime;
};
