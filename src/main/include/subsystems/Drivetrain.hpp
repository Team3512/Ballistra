// Copyright (c) 2014-2021 FRC Team 3512. All Rights Reserved.

#pragma once

#include <frc/Encoder.h>
#include <frc/Solenoid.h>
#include <frc/SpeedControllerGroup.h>
#include <frc/Talon.h>
#include <frc/controller/ProfiledPIDController.h>
#include <frc/drive/DifferentialDrive.h>
#include <frc/trajectory/TrapezoidProfile.h>
#include <units/acceleration.h>
#include <units/length.h>
#include <units/velocity.h>

class Drivetrain {
public:
    Drivetrain();

    void Drive(double xSpeed, double zRotation, bool isQuickTurn);

    /**
     * Set encoder distances to 0.
     */
    void ResetEncoders();

    /**
     * Set left wheel position goal.
     */
    void SetLeftGoal(units::meter_t goal);

    /**
     * Set right wheel position goal.
     */
    void SetRightGoal(units::meter_t goal);

    /**
     * Returns left encoder distance.
     */
    units::inch_t GetLeftDist() const;

    /**
     * Returns right encoder distance.
     */
    units::inch_t GetRightDist() const;

    /**
     * Code to run in TimedRobot::TeleopPeriodic().
     */
    void TeleopPeriodic();

private:
    bool m_isDefensive = false;
    frc::Encoder m_leftEncoder{5, 6, true};
    frc::Encoder m_rightEncoder{3, 4};

    frc::Solenoid m_shifter{7};

    frc::ProfiledPIDController<units::meter> m_leftController{
        0.2, 0, 0, {5_mps, 10_mps_sq}};
    frc::ProfiledPIDController<units::meter> m_rightController{
        0.2, 0, 0, {5_mps, 10_mps_sq}};

    frc::Talon m_fl{1};
    frc::Talon m_ml{2};
    frc::Talon m_rl{3};
    frc::SpeedControllerGroup m_leftGrbx{m_fl, m_ml, m_rl};

    frc::Talon m_fr{4};
    frc::Talon m_mr{5};
    frc::Talon m_rr{6};
    frc::SpeedControllerGroup m_rightGrbx{m_fr, m_mr, m_rr};

    frc::DifferentialDrive m_robotDrive{m_leftGrbx, m_rightGrbx};
};
