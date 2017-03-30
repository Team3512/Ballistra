// Copyright (c) FRC Team 3512, Spartatroniks 2014-2017. All Rights Reserved.

#include "LogConsoleSink.h"

#include <iostream>

void LogConsoleSink::logEvent(LogEvent event) {
    std::cout << event.toFormattedString();
}
