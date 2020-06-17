// Copyright (c) 2017-2021 FRC Team 3512. All Rights Reserved.

#include "Robot.hpp"

enum class State {
    kInit,
    kAutonInit,
    kAutonShooting,
    kAutonCalculate,
    kAutonUpdate,
    kIdle
};
static State state;

void Robot::AutonMotionProfileInit() { state = State::kInit; }

void Robot::AutonMotionProfilePeriodic() {
    switch (state) {
        case State::kInit: {
            m_drivetrain.ResetEncoders();
            m_claw.SetAngleReference(88_deg);

            state = State::kInit;
            break;
        }

        case State::kAutonInit: {
            // Move robot 360 cm forward
            m_drivetrain.SetLeftGoal(360_cm);
            m_drivetrain.SetRightGoal(360_cm);

            // Check if moving to kAutonCalculate state is necessary
            if (IsAutonomous() && IsEnabled()) {
                state = State::kAutonCalculate;
                break;
            }

            state = State::kAutonShooting;
            break;
        }

        case State::kAutonCalculate: {
            state = State::kAutonShooting;
            break;
        }

        case State::kAutonShooting: {
            // Stop moving
            m_drivetrain.Drive(0.0, 0.0, false);

            // Start shooting
            m_claw.Shoot();

            if (IsAutonomous() && IsEnabled() && m_claw.IsShooting()) {
                state = State::kAutonUpdate;
                break;
            }
            state = State::kIdle;
            break;
        }

        case State::kAutonUpdate: {
            state = State::kIdle;
            break;
        }

        case State::kIdle:
            break;
    }
}
