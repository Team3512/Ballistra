// Copyright (c) 2014-2021 FRC Team 3512. All Rights Reserved.

#include "subsystems/Claw.hpp"

#include <frc/DriverStation.h>
#include <frc/Joystick.h>
#include <wpi/math>

Claw::Claw() {
    // Sets degrees rotated per pulse of encoder
    m_angleEncoder.SetDistancePerPulse((1.0 / 71.0) * 14.0 / 44.0);

    m_ballShooter.emplace_back(8);
    m_ballShooter.emplace_back(2);
    m_ballShooter.emplace_back(3);
    m_ballShooter.emplace_back(6);
}

void Claw::SetAngleReference(units::degree_t shooterAngle) {
    m_controller.SetSetpoint(shooterAngle.to<double>());
}

units::degree_t Claw::GetAngleReference() const {
    return units::degree_t{m_controller.GetSetpoint()};
}

void Claw::SetWheel(double speed) { m_intakeWheel.Set(speed); }

void Claw::Shoot() {
    if (m_shooterState == ShooterState::kIdle) {
        m_collectorArm.Set(true);
        m_shooterState = ShooterState::kArmIsLifting;
        m_shootTimer.Start();
        m_shootTimer.Reset();
    }
}

bool Claw::IsShooting() const { return m_shooterState != ShooterState::kIdle; }

void Claw::RobotPeriodic() {
    static frc::Joystick driveStick2{2};
    static frc::Joystick shootStick{3};

    if (driveStick2.GetRawButtonPressed(7)) {
        SetAngleReference(190_deg);
    }

    if (driveStick2.GetRawButtonPressed(9)) {
        SetAngleReference(106_deg);
    }

    if (driveStick2.GetRawButtonPressed(8)) {
        SetAngleReference(57_deg);
    }

    if (driveStick2.GetRawButtonPressed(11)) {
        // Collector should always be retracted when resetting encoder
        m_collectorArm.Set(false);
        SetAngleReference(0_deg);
    }

    if (driveStick2.GetRawButtonPressed(10)) {
        SetAngleReference(GetAngleReference() + 3_deg);
    }

    if (driveStick2.GetRawButtonPressed(12)) {
        SetAngleReference(GetAngleReference() - 3_deg);
    }

    // Engage collector
    if (driveStick2.GetRawButtonPressed(2)) {
        m_collectorArm.Set(!m_collectorArm.Get());
    }

    // Shoots a ball
    if ((shootStick.GetRawButtonPressed(1) ||
         driveStick2.GetRawButtonPressed(1)) &&
        !IsShooting()) {
        Shoot();
    }

    if (shootStick.GetRawButtonPressed(3)) {
        SetWheel(-1.0);
    } else if (shootStick.GetRawButtonPressed(4)) {
        SetWheel(1.0);
    } else {
        SetWheel(0.0);
    }

    double ff = 0.0;
    if (m_controller.GetSetpoint() > 0.0) {
        ff = kK *
             std::cos((m_angleEncoder.GetDistance() + kL) * wpi::math::pi /
                      180.0) /
             m_controller.GetSetpoint();
    }
    double fb = m_controller.Calculate(m_angleEncoder.GetDistance());

    m_clawRotator.Set(ff + fb);

    if (m_shooterState == ShooterState::kArmIsLifting &&
        m_shootTimer.HasPeriodPassed(0.5_s)) {
        for (auto& solenoid : m_ballShooter) {
            solenoid.Set(true);
        }
        m_shootTimer.Reset();
        m_shooterState = ShooterState::kShooting;
    }
    if (m_shooterState == ShooterState::kShooting &&
        m_shootTimer.HasPeriodPassed(2.0_s)) {
        for (auto& solenoid : m_ballShooter) {
            solenoid.Set(false);
        }
        m_vacuum.Set(frc::Relay::kOn);
        m_shootTimer.Reset();
        m_shooterState = ShooterState::kVacuuming;
    }
    if (m_shooterState == ShooterState::kVacuuming &&
        m_shootTimer.HasPeriodPassed(3.0_s)) {
        m_vacuum.Set(frc::Relay::kOff);
        m_collectorArm.Set(false);

        m_shootTimer.Reset();
        m_shooterState = ShooterState::kIdle;
    }

    // Spins intake wheel to keep ball in while rotating claw at high speeds
    if (std::abs(m_angleEncoder.GetRate()) > 35.0) {
        SetWheel(-1.0);
    }

    /* Fixes arm, when at reset angle, not touching zeroSwitch due to gradual
     * encoder error. If limit switch isn't pressed but arm is supposedly at
     * zeroing point or farther:
     */
    if (m_zeroSwitch.Get() && GetAngleReference() <= 1_deg) {
        SetAngleReference(GetAngleReference() - 5_deg);
    }

    // If wasn't pressed last time and is now
    if (!m_zeroSwitch.Get() && GetAngleReference() <= 0_deg) {
        SetAngleReference(0_deg);
        m_controller.Reset();
        m_angleEncoder.Reset();
    }

    // Close claw if zero switch is pressed
    if (m_zeroSwitch.Get() && !IsShooting()) {
        m_collectorArm.Set(false);
    }

    m_lastZeroSwitch = m_zeroSwitch.Get();
}

void Claw::TestClaw() {
    auto& ds = frc::DriverStation::GetInstance();

    for (auto& solenoid : m_ballShooter) {
        solenoid.Set(true);
        frc2::Wait(1.5_s);

        solenoid.Set(false);
        frc2::Wait(1.5_s);

        m_vacuum.Set(frc::Relay::kOn);
        frc2::Wait(1.5_s);

        m_vacuum.Set(frc::Relay::kOff);
        frc2::Wait(1.5_s);

        if (ds.IsDisabled()) {
            return;
        }
    }
}
