// Copyright (c) 2014-2021 FRC Team 3512. All Rights Reserved.

#pragma once

#include <frc/TimedRobot.h>
#include <frc2/Timer.h>
#include <wpi/raw_ostream.h>

#include "AutonomousChooser.hpp"
#include "subsystems/Claw.hpp"
#include "subsystems/Drivetrain.hpp"

class Robot : public frc::TimedRobot {
public:
    Robot();

    void AutonomousInit() override;

    void RobotPeriodic() override;
    void AutonomousPeriodic() override;
    void TeleopPeriodic() override;
    void TestPeriodic() override;

    bool CheckReflectiveStrips();

    void AutonRightLeftInit();
    void AutonRightLeftPeriodic();
    void AutonMotionProfileInit();
    void AutonMotionProfilePeriodic();
    void AutonDriveForwardInit();
    void AutonDriveForwardPeriodic();
    void AutonSideInit();
    void AutonSidePeriodic();

private:
    Drivetrain m_drivetrain;
    Claw m_claw;

    frc2::Timer autonTimer;

    AutonomousChooser m_autonChooser{
        "No-op", [] { wpi::outs() << "No-op autonomous\n"; }, [] {}};
};
