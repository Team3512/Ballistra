// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

#pragma once

template <class T>
T DriveTrain::limit(T value, T limit) {
    if (value > limit) {
        return limit;
    } else if (value < -limit) {
        return -limit;
    } else {
        return value;
    }
}
