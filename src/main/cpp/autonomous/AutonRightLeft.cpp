// Copyright (c) 2017-2021 FRC Team 3512. All Rights Reserved.

#include "Robot.hpp"

enum class State {
    kInit,
    kWait,
    kAutonMove,
    kAutonTargetDrive,
    kAutonShoot,
    kIdle
};
static State state;

void Robot::AutonRightLeftInit() { state = State::kInit; }

void Robot::AutonRightLeftPeriodic() {
    frc2::Timer timer;
    constexpr auto kTargetDistance = 295_in;
    bool targetLit = CheckReflectiveStrips();

    switch (state) {
        case State::kInit: {
            // Resets encoders and timer
            timer.Reset();
            m_drivetrain.ResetEncoders();
            timer.Start();

            state = State::kWait;
            break;
        }

        case State::kWait: {
            if (timer.HasPeriodPassed(0.5_s)) {
                state = State::kAutonMove;
            }
            break;
        }

        case State::kAutonMove: {
            // Moves robot and passes to other states if certain conditions are
            // met
            if (timer.HasPeriodPassed(0.5_s)) {
                m_drivetrain.Drive(-0.1, 0, false);
            } else {
                m_drivetrain.Drive(0, 0, false);
            }

            m_claw.SetAngleReference(115_deg);

            if (m_drivetrain.GetRightDist() < kTargetDistance &&
                m_drivetrain.GetRightDist() > -5_in) {
                state = State::kAutonTargetDrive;
                break;
            } else if (timer.HasPeriodPassed(0.1_s)) {
                state = State::kAutonShoot;
                break;
            }
            state = State::kAutonShoot;
            break;
        }

        case State::kAutonTargetDrive: {
            double targetValue =
                -0.6 * (1.0 - m_drivetrain.GetRightDist() / kTargetDistance);
            m_drivetrain.Drive(targetValue, 0, false);

            state = State::kAutonShoot;
            break;
        }

        case State::kAutonShoot: {
            // drives robot + set the wheel manuel
            m_drivetrain.Drive(0, 0, false);

            if (m_drivetrain.GetLeftDist() < m_drivetrain.GetRightDist()) {
                m_drivetrain.Drive(0.3, 0.0, false);
            }

            m_claw.SetWheel(0.0);

            if (timer.HasPeriodPassed(0.1_s)) {
                m_drivetrain.Drive(0, 0, false);
            } else {
                m_drivetrain.Drive(-0.1, 0, false);
            }

            if (targetLit) {
                m_claw.Shoot();
            } else if (timer.HasPeriodPassed(5.01_s)) {
                m_claw.Shoot();
            }

            if (timer.HasPeriodPassed(0.1_s)) {
                state = State::kIdle;
                break;
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
