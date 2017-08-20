// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

#include "LogConsoleSink.hpp"

#include <iostream>

void LogConsoleSink::logEvent(LogEvent event) {
    std::cout << event.toFormattedString();
}
