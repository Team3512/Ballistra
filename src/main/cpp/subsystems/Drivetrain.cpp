// Copyright (c) 2014-2021 FRC Team 3512. All Rights Reserved.

#include "subsystems/Drivetrain.hpp"

#include <frc/Joystick.h>
#include <wpi/math>

Drivetrain::Drivetrain() {
    constexpr auto kWheelDiameter = 4_in;
    constexpr double kDpP = wpi::math::pi * kWheelDiameter.to<double>() / 360.0;
    m_leftEncoder.SetDistancePerPulse(kDpP);
    m_rightEncoder.SetDistancePerPulse(kDpP);
}

void Drivetrain::Drive(double xSpeed, double zRotation, bool isQuickTurn) {
    m_robotDrive.CurvatureDrive(xSpeed, zRotation, isQuickTurn);
}

void Drivetrain::ResetEncoders() {
    m_leftEncoder.Reset();
    m_rightEncoder.Reset();
}

void Drivetrain::SetLeftGoal(units::meter_t goal) {
    m_leftController.SetGoal(goal);
}

void Drivetrain::SetRightGoal(units::meter_t goal) {
    m_rightController.SetGoal(goal);
}

units::inch_t Drivetrain::GetLeftDist() const {
    return units::inch_t{m_leftEncoder.GetDistance()};
}

units::inch_t Drivetrain::GetRightDist() const {
    return units::inch_t{m_rightEncoder.GetDistance()};
}

void Drivetrain::TeleopPeriodic() {
    static frc::Joystick driveStick1{1};
    static frc::Joystick driveStick2{2};

    if (driveStick1.GetRawButtonPressed(1)) {
        m_shifter.Set(!m_shifter.Get());
    }

    if (driveStick1.GetRawButtonPressed(2)) {
        m_isDefensive = !m_isDefensive;
    }

    if (m_isDefensive) {
        Drive(-driveStick1.GetY(), -driveStick2.GetZ(),
              driveStick2.GetRawButtonPressed(2));
    } else {
        Drive(driveStick1.GetY(), driveStick2.GetZ(),
              driveStick2.GetRawButtonPressed(2));
    }
}
