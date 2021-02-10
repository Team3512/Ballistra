// Copyright (c) 2017-2021 FRC Team 3512. All Rights Reserved.

#include <frc2/Timer.h>

#include "Robot.hpp"

void Robot::AutonDriveForward() {
    frc2::Timer timer;
    timer.Start();

    // Drive 1
    while (!timer.HasPeriodPassed(0.5_s)) {
        m_drivetrain.Drive(-0.1, 0, false);
        m_autonChooser.YieldToMain();
        if (!IsAutonomousEnabled()) {
            return;
        }
    }

    // Drive 2
    while (!timer.HasPeriodPassed(0.5_s)) {
        m_drivetrain.Drive(-0.5, 0, false);
        m_autonChooser.YieldToMain();
        if (!IsAutonomousEnabled()) {
            return;
        }
    }
    m_drivetrain.Drive(0, 0, false);
}
