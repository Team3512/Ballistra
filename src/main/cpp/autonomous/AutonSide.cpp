// Copyright (c) 2017-2021 FRC Team 3512. All Rights Reserved.

#include <frc2/Timer.h>

#include "Robot.hpp"

void Robot::AutonSide() {
    constexpr units::inch_t kTargetDistance{-430.0};

    frc2::Timer timer;
    timer.Start();

    bool targetLit = CheckReflectiveStrips();

    m_drivetrain.ResetEncoders();

    // Wait
    while (!timer.HasPeriodPassed(0.5_s)) {
        m_autonChooser.YieldToMain();
        if (!IsAutonomousEnabled()) {
            return;
        }
    }

    while (!timer.HasPeriodPassed(0.25_s)) {
        m_drivetrain.Drive(-0.1, 0, false);

        m_autonChooser.YieldToMain();
        if (!IsAutonomousEnabled()) {
            return;
        }
    }

    m_claw.SetAngleReference(39_deg);

    while (!timer.HasPeriodPassed(0.5_s)) {
        m_autonChooser.YieldToMain();
        if (!IsAutonomousEnabled()) {
            return;
        }
    }

    while (m_drivetrain.GetRightDist() < kTargetDistance &&
           m_drivetrain.GetRightDist() > -5_in) {
        double targetValue =
            -0.6 * (1.0 - m_drivetrain.GetRightDist() / kTargetDistance);
        m_drivetrain.Drive(targetValue, 0, false);

        m_autonChooser.YieldToMain();
        if (!IsAutonomousEnabled()) {
            return;
        }
    }

    // Rotate robot to straighten it out
    while (-m_drivetrain.GetLeftDist() < m_drivetrain.GetRightDist()) {
        m_drivetrain.Drive(0.0, 0.3, true);

        m_autonChooser.YieldToMain();
        if (!IsAutonomousEnabled()) {
            return;
        }
    }

    m_claw.SetWheel(0.0);

    timer.Reset();
    while (!timer.HasPeriodPassed(0.1_s)) {
        m_drivetrain.Drive(-0.1, 0.0, false);

        m_autonChooser.YieldToMain();
        if (!IsAutonomousEnabled()) {
            return;
        }
    }

    if (!targetLit) {
        while (!timer.HasPeriodPassed(0.5_s)) {
            m_autonChooser.YieldToMain();
            if (!IsAutonomousEnabled()) {
                return;
            }
        }
    }

    m_claw.Shoot();

    while (m_claw.IsShooting()) {
        m_autonChooser.YieldToMain();
        if (!IsAutonomousEnabled()) {
            return;
        }
    }
}
