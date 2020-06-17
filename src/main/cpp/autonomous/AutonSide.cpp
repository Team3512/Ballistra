// Copyright (c) 2017-2021 FRC Team 3512. All Rights Reserved.

#include "Robot.hpp"

enum class State {
    kInit,
    kWait,
    kAutonDrive,
    kAutonTarget,
    kAutonManuel,
    kAutonShoot,
    kAutonUpdate,
    kIdle
};
static State state;

void Robot::AutonSideInit() { state = State::kInit; }

void Robot::AutonSidePeriodic() {
    frc2::Timer timer;
    constexpr units::inch_t kTargetDistance{430.0};
    bool targetLit = CheckReflectiveStrips();

    switch (state) {
        case State::kInit: {
            // Reset timer and encoders
            timer.Reset();
            m_drivetrain.ResetEncoders();
            timer.Start();

            state = State::kWait;
            break;
        }

        case State::kWait: {
            if (timer.HasPeriodPassed(0.5_s)) {
                state = State::kAutonDrive;
            }
            break;
        }

        case State::kAutonDrive: {
            m_drivetrain.Drive(-0.1, 0, false);

            if (timer.HasPeriodPassed(0.25_s)) {
                m_claw.SetAngleReference(39_deg);
            }

            if (timer.HasPeriodPassed(0.5_s)) {
                if (m_drivetrain.GetRightDist() > kTargetDistance &&
                    IsEnabled() && IsAutonomous()) {
                    state = State::kAutonManuel;
                } else {
                    state = State::kAutonTarget;
                }
            }
            break;
        }

        case State::kAutonTarget: {
            double targetValue =
                0.6 * (1.0 - m_drivetrain.GetRightDist() / kTargetDistance);
            m_drivetrain.Drive(targetValue, 0, false);

            if (timer.HasPeriodPassed(0.1_s)) {
                state = State::kAutonDrive;
            } else {
                state = State::kAutonShoot;
            }
            break;
        }

        case State::kAutonManuel: {
            m_drivetrain.Drive(0, 0, false);

            if (timer.HasPeriodPassed(0.1_s)) {
                state = State::kAutonShoot;
            }
            state = State::kAutonShoot;
            break;
        }

        case State::kAutonUpdate: {
            if (timer.HasPeriodPassed(0.1_s)) {
                state = State::kIdle;
            }
            state = State::kIdle;
            break;
        }

        case State::kAutonShoot: {
            if (m_drivetrain.GetLeftDist() > m_drivetrain.GetRightDist()) {
                state = State::kAutonManuel;
            } else {
                m_drivetrain.Drive(0, 0, false);

                if (timer.HasPeriodPassed(0.1_s)) {
                    m_drivetrain.Drive(0, 0, false);
                } else {
                    m_drivetrain.Drive(0, 0, false);
                }

                if (targetLit) {
                    m_claw.Shoot();
                } else {
                    if (timer.HasPeriodPassed(5.01_s)) {
                        m_claw.Shoot();
                    }
                }
                if (IsEnabled() && IsAutonomous()) {
                    state = State::kAutonUpdate;
                }
            }
            state = State::kIdle;
            break;
        }

        case State::kIdle: {
            timer.Stop();
            break;
        }
    }
}
