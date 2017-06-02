// Copyright (c) 2017 FRC Team 3512. All Rights Reserved.

#include <Timer.h>

#include "../Robot.hpp"

void Robot::DriveForwardAuton() {
    robotDrive->drive(0, 0);
    Wait(0.5);
    robotDrive->drive(-0.1, 0);
    Wait(0.5);
    robotDrive->drive(-0.5, 0);
    Wait(0.5);
    robotDrive->drive(0, 0);
}
