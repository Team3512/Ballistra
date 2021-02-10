// Copyright (c) 2017-2021 FRC Team 3512. All Rights Reserved.

#include "Robot.hpp"

Robot::Robot() {
    m_autonChooser.AddAutonomous("DriveForward Autonomous",
                                 [=] { AutonDriveForward(); });
    m_autonChooser.AddAutonomous("Right/Left Autonomous",
                                 [=] { AutonRightLeft(); });
    m_autonChooser.AddAutonomous("Side Auton", [=] { AutonSide(); });
}

void Robot::DisabledInit() { m_autonChooser.EndAutonomous(); }

void Robot::AutonomousInit() { m_autonChooser.AwaitStartAutonomous(); }

void Robot::TeleopInit() { m_autonChooser.EndAutonomous(); }

void Robot::TestInit() { m_autonChooser.EndAutonomous(); }

void Robot::RobotPeriodic() { m_claw.RobotPeriodic(); }

void Robot::AutonomousPeriodic() { m_autonChooser.AwaitRunAutonomous(); }

void Robot::TeleopPeriodic() { m_drivetrain.TeleopPeriodic(); }

void Robot::TestPeriodic() { m_claw.TestClaw(); }

bool Robot::CheckReflectiveStrips() { return true; }

#ifndef RUNNING_FRC_TESTS
int main() { return frc::StartRobot<Robot>(); }
#endif
