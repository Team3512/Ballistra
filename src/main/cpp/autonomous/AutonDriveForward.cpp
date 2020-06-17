// Copyright (c) 2017-2021 FRC Team 3512. All Rights Reserved.

#include "Robot.hpp"

enum class State { kInit, kDrive1, kDrive2, kExit };
static State state;

void Robot::AutonDriveForwardInit() { state = State::kInit; }

void Robot::AutonDriveForwardPeriodic() {
    frc2::Timer timer;

    switch (state) {
        case State::kInit: {
            state = State::kDrive1;
            break;
        }

        case State::kDrive1: {
            timer.Start();

            m_drivetrain.Drive(0, 0, false);
            if (timer.HasPeriodPassed(0.5_s)) {
                m_drivetrain.Drive(-0.1, 0, false);
            }

            state = State::kDrive2;
            break;
        }

        case State::kDrive2: {
            m_drivetrain.Drive(-0.5, 0, false);
            if (timer.HasPeriodPassed(0.5_s)) {
                m_drivetrain.Drive(0, 0, false);
            }

            timer.Stop();
            timer.Reset();

            state = State::kExit;
            break;
        }

        case State::kExit:
            break;
    }
}
