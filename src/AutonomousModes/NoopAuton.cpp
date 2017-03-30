// Copyright (c) FRC Team 3512, Spartatroniks 2017. All Rights Reserved.

#include <Timer.h>

#include "../Robot.hpp"

void Robot::NoopAuton() {
    while (IsAutonomous()) {
        Wait(0.01);
    }
}
