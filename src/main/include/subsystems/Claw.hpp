// Copyright (c) 2014-2021 FRC Team 3512. All Rights Reserved.

#pragma once

#include <vector>

#include <frc/DigitalInput.h>
#include <frc/Encoder.h>
#include <frc/Relay.h>
#include <frc/Solenoid.h>
#include <frc/Talon.h>
#include <frc/controller/PIDController.h>
#include <frc2/Timer.h>
#include <units/angle.h>

class Claw {
public:
    Claw();

    /**
     * Set reference angle of claw.
     */
    void SetAngleReference(units::degree_t shooterAngle);

    /**
     * Returns claw angle reference.
     */
    units::degree_t GetAngleReference() const;

    /**
     * Sets speed of claw's intake wheel.
     */
    void SetWheel(double speed);

    /**
     * Activates solenoids to shoot ball.
     */
    void Shoot();

    /**
     * Returns true if this instance is in the middle of shooting.
     */
    bool IsShooting() const;

    /**
     * Code to run in TimedRobot::RobotPeriodic().
     */
    void RobotPeriodic();

    void TestClaw();

private:
    enum class ShooterState { kIdle, kShooting, kVacuuming, kArmIsLifting };

    static constexpr double kK = 0.238;
    static constexpr double kL = 69.0;

    frc::Talon m_clawRotator{7};
    frc::Talon m_intakeWheel{8};

    frc::Encoder m_angleEncoder{7, 8};

    frc2::PIDController m_controller{0.098, 0.08, 0.01};

    // Resets the angle encoder to 0
    frc::DigitalInput m_zeroSwitch{2};

    // Returns true when ball is hitting limit switch in claw
    frc::DigitalInput m_haveBallSwitch{9};

    frc2::Timer m_shootTimer;
    ShooterState m_shooterState = ShooterState::kIdle;

    std::vector<frc::Solenoid> m_ballShooter;
    frc::Relay m_vacuum{2, frc::Relay::kForwardOnly};
    frc::Solenoid m_collectorArm{5};

    bool m_lastZeroSwitch = true;
};
