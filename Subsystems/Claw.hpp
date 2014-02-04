#ifndef CLAW_HPP
#define CLAW_HPP

#include <vector>

#include <Encoder.h>
#include <Talon.h>
#include <Timer.h>

#include "../Settings.hpp"
#include "GearBox.hpp"

class Solenoid;

class Claw {
public:
    Claw (float clawRotatePort,float clawWheelPort);
    ~Claw();

    // Set angle of claw in degrees
    void SetAngle( float shooterAngle );

    // Returns setpoint of rotator's internal PID loop in degrees
    double GetTargetAngle() const;

    // Sets speed of claw's intake wheel
    void SetWheelSetpoint( float speed );
    void SetWheelManual( float speed );

    double GetWheelSetpoint() const;

    // Set encoder distances to 0
    void ResetEncoders();

    // Reload PID constants
    void ReloadPID();

    // Starts activating solenoids to shoot ball
    void Shoot();

    // Continues process of shooting
    void Update();

    // Returns true if this instance is in the middle of shooting
    bool IsShooting() const;

private:
    Settings m_settings;

    GearBox<Talon>* m_clawRotator;
    GearBox<Talon>* m_intakeWheel;

    Timer m_shootTimer;
    bool m_isShooting;

    std::vector<Solenoid*> m_ballShooter;
};

#endif // CLAW_HPP
