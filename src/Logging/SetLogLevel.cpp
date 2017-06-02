// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

#include "SetLogLevel.h"

SetLogLevel::SetLogLevel(int level) { m_level = level; }

LogStream& operator<<(LogStream& os, const SetLogLevel& in) {
    os.setLevel(in.m_level);
    return os;
}
