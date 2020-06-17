// Copyright (c) 2017-2021 FRC Team 3512. All Rights Reserved.

#include "Robot.hpp"

Robot::Robot() {
    m_autonChooser.AddAutonomous(
        "DriveForward Autonomous", [=] { AutonDriveForwardInit(); },
        [=] { AutonDriveForwardPeriodic(); });
    m_autonChooser.AddAutonomous(
        "Right/Left Autonomous", [=] { AutonRightLeftInit(); },
        [=] { AutonRightLeftPeriodic(); });
    m_autonChooser.AddAutonomous(
        "MotionProfile", [=] { AutonMotionProfileInit(); },
        [=] { AutonMotionProfilePeriodic(); });
    m_autonChooser.AddAutonomous(
        "Side Auton", [=] { AutonSideInit(); }, [=] { AutonSidePeriodic(); });
}

void Robot::AutonomousInit() {
    autonTimer.Reset();
    autonTimer.Start();

    m_drivetrain.ResetEncoders();
    m_autonChooser.RunAutonomousInit();
}

void Robot::RobotPeriodic() { m_claw.RobotPeriodic(); }

void Robot::AutonomousPeriodic() { m_autonChooser.RunAutonomousPeriodic(); }

void Robot::TeleopPeriodic() { m_drivetrain.TeleopPeriodic(); }

void Robot::TestPeriodic() { m_claw.TestClaw(); }

bool Robot::CheckReflectiveStrips() { return true; }

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
