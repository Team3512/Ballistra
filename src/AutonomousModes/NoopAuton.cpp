// Copyright (c) 2017 FRC Team 3512. All Rights Reserved.

#include <Timer.h>

#include "../Robot.hpp"

void Robot::NoopAuton() {
    while (IsAutonomous()) {
        Wait(0.01);
    }
}
