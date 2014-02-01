#ifndef CLAW_HPP
#define CLAW_HPP

#include <vector>

#include <Encoder.h>
#include <Talon.h>
#include <Solenoid.h>
#include <Timer.h>

class Claw {
public:
    Claw (float clawAnglePort,float clawShooterWheelPort);
    ~Claw();

    void SetAngle(float shooterAngle);
    void SetWheel(float wheelSpeed);

    // Starts activating solenoids to shoot ball
    void Shoot();

    // Continues process of shooting
    void Update();

    // Returns true if this instance is in the middle of shooting
    bool IsShooting() const;

private:
    Talon m_shooterWheel;
    Talon m_shooterBase;
    Encoder m_wheelEncoder;
    Encoder m_shooterEncoder;

    Timer m_shootTimer;
    bool m_isShooting;

    std::vector<Solenoid*> m_ballShooter;
};

#endif // CLAW_HPP
