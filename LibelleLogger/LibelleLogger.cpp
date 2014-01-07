/*
 * LibelleLogger.cpp
 *
 *  Created on: Nov 10, 2013
 *      Author: acf
 */

#include "LibelleLogger.h"
#include <sstream>

LibelleLogger::LibelleLogger()
{
    /* Capture all events by default */
    verbosity = LogEvent::VERBOSE_ALL;

    /* Initialize the logger */
    m_logger = new Logger();

    /* Initialize the logger stream */
    m_stream = new LogStream(m_logger);

    /* Initialize the console output */
    m_consoleSink = new LogConsoleSink();
    m_logger->addLogSink(m_consoleSink);
    m_consoleSink->setVerbosityLevels(verbosity);

    /* Initialize the file output */
    time_t rawtime;
    struct tm *timeinfo;
    std::stringstream logFilePath;
    char timestr[128];

    /* Generate the path to the logfile based
     * on the current time.
     */
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(timestr, 128, "%Y-%m-%d_%H%M%S", timeinfo);
    logFilePath << "logs/" << std::string(timestr) << ".txt";

    /* Initialize the logger file sink */
    m_fileSink = new LogFileSink(logFilePath.str());
    m_logger->addLogSink(m_fileSink);
    m_fileSink->setVerbosityLevels(verbosity);

    /* Logger initialized */
    logEvent(LogEvent("Libelle",
            LogEvent::VERBOSE_INFO));
    logEvent(LogEvent("E = Error  W = Warning  I = Information  "
            "U = User  D = Debug  X = Other",
            LogEvent::VERBOSE_INFO));
    logEvent(LogEvent("Debug levels enabled: " + LogEvent::verbosityLevelString(verbosity),
            LogEvent::VERBOSE_INFO));

    /* Log the current time */
    strftime(timestr, 128, "%a %b %d %H:%M:%S %Z %Y", timeinfo);
    logEvent(LogEvent(std::string(timestr),
            LogEvent::VERBOSE_INFO));

    logEvent(LogEvent("Log file at " + logFilePath.str(),
            LogEvent::VERBOSE_INFO));

}

LibelleLogger::~LibelleLogger()
{

    /* De-initialize the logging engine */
    m_logger->removeLogSink(m_consoleSink);
    m_logger->removeLogSink(m_fileSink);
    delete m_consoleSink;
    delete m_fileSink;
    delete m_stream;
    delete m_logger;
}

void LibelleLogger::logEvent(LogEvent event)
{
    m_logger->logEvent(event);
}

LogStream& LibelleLogger::st() {
    return *m_stream;
}

void LibelleLogger::setVerbosity(LogEvent::VerbosityLevel verbosity)
{
    m_consoleSink->setVerbosityLevels(verbosity);
    m_fileSink->setVerbosityLevels(verbosity);
    logEvent(LogEvent("Enabled debug levels changed: " + LogEvent::verbosityLevelString(verbosity),
            LogEvent::VERBOSE_INFO));
}
