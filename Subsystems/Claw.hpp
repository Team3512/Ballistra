#ifndef CLAW_HPP
#define CLAW_HPP

#include <vector>

#include <Encoder.h>
#include <Talon.h>
#include <Timer.h>
#include <DriverStationLCD.h>

#include "../Settings.hpp"
#include "GearBox.hpp"

class Solenoid;

class Claw {
public:
    Claw (float clawRotatePort,float clawWheelPort);
    ~Claw();

    // Set mode of collector
    void SetCollectorMode( bool collectorMode );

    bool GetCollectorMode();

    // Set angle of claw in degrees
    void SetAngle( float shooterAngle );

    // Manually set the value of the angle motor
    void ManualSetAngle(float value);

	// Returns setpoint of rotator's internal PID loop in degrees
    double GetTargetAngle() const;

    // Sets speed of claw's intake wheel
    void SetWheelSetpoint( float speed );
    void SetWheelManual( float speed );

    double GetWheelSetpoint() const;

    double getDistance();

    // Set encoder distances to 0
    void ResetEncoders();

    // Reload PID constants
    void ReloadPID();

    // StartShoots activating solenoids to shoot ball
    void Shoot();

    // Continues process of shooting
    void Update();

    // Returns true if this instance is in the middle of shooting
    bool IsShooting() const;

    void setK(float k);

    void setF(float f);

    float calcF();

private:
    Settings m_settings;
    float m_k;
    float m_l;

    GearBox<Talon>* m_clawRotator;
    GearBox<Talon>* m_intakeWheel;

    Timer m_shootTimer;
    Timer vacuumTimer;
    bool m_isShooting;
    bool m_isVacuuming;

    std::vector<Solenoid*> m_ballShooter;
    Solenoid *vacuum;
    Solenoid *collectorArm;

};

#endif // CLAW_HPP
