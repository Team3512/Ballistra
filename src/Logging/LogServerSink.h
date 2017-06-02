// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

#pragma once

#include <list>
#include "LogSinkBase.h"

/*!
  A file sink for the logged events.
 */
class LogServerSink : public LogSinkBase {
public:
    virtual ~LogServerSink();

    /*!
      Write an event to the logfile.
      \param event The event to log.
     */
    void logEvent(LogEvent event);

    int startServer(unsigned short port);

    int acceptor(bool blocking);

private:
    int tcpListen(unsigned short port);
    int acceptConnectionBlocking();
    int acceptConnectionNonBlocking();

    int m_listensd = -1;
    std::list<int> m_connections;
};
