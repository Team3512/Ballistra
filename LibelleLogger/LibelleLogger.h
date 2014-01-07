/*
 * LibelleLogger.h
 *
 *  Created on: Nov 10, 2013
 *      Author: acf
 */

#ifndef LIBELLELOGGER_H_
#define LIBELLELOGGER_H_

#include "Logging/Logger.h"
#include "Logging/LogConsoleSink.h"
#include "Logging/LogFileSink.h"
#include "Logging/LogStream.h"
#include "Logging/SetLogLevel.h"

class LibelleLogger
{
public:
    LibelleLogger();
    virtual ~LibelleLogger();

    void logEvent(LogEvent event);
    void setVerbosity(LogEvent::VerbosityLevel verbosity);
    LogStream& st();

private:
    /* The verbosity levels for which we will accept messages */
    LogEvent::VerbosityLevel verbosity;

    /* The logger */
    Logger *m_logger;

    /* std::ostream implementation for the logger */
    LogStream *m_stream;

    /* Console output for m_logger */
    LogConsoleSink *m_consoleSink;

    /* File output for m_logger */
    LogFileSink *m_fileSink;
};

#endif /* LIBELLELOGGER_H_ */
