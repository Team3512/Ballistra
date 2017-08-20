// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

#pragma once

#include <iostream>
#include <streambuf>
#include <string>

#include "LogEvent.hpp"
#include "Logger.hpp"

/*!
  An internal class used by LogStream. This class
  implements the interface with the Logger class.
 */
class LogStreambuf : public std::streambuf {
public:
    explicit LogStreambuf(Logger* logger);
    virtual ~LogStreambuf() = default;

    std::streamsize xsputn(const char* s, std::streamsize n);

    /*!
      Called when the stream is flushed. This can occur when
      the user pipes std::flush or std::endl into the stream.
      This function generates an event from the accumulated
      information (message, verbosity level, etc...) and calls
      the given logEvent function of the given Logger class
      instance.
      \return Always returns 0
     */
    int sync();

    /*!
      The setLevel function is called by the setLevel member
      of the LogStream class. It simply stores the provided
      verbosity level for use when a flush event occurs on
      the stream (causing the sync() function to be called).
      \see sync()
     */
    void setLevel(LogEvent::VerbosityLevel level);

private:
    std::string m_buf;
    Logger* m_logger;
    LogEvent::VerbosityLevel m_level = LogEvent::VERBOSE_NONE;
};
