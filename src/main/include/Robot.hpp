// Copyright (c) 2014-2021 FRC Team 3512. All Rights Reserved.

#pragma once

#include <frc/TimedRobot.h>

#include "AutonomousChooser.hpp"
#include "subsystems/Claw.hpp"
#include "subsystems/Drivetrain.hpp"

class Robot : public frc::TimedRobot {
public:
    Robot();

    void DisabledInit() override;
    void AutonomousInit() override;
    void TeleopInit() override;
    void TestInit() override;

    void RobotPeriodic() override;
    void AutonomousPeriodic() override;
    void TeleopPeriodic() override;
    void TestPeriodic() override;

    bool CheckReflectiveStrips();

    void AutonRightLeft();
    void AutonDriveForward();
    void AutonSide();

private:
    Drivetrain m_drivetrain;
    Claw m_claw;

    frc3512::AutonomousChooser m_autonChooser{"No-op", [] {}};
};
