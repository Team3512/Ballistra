#include <Encoder.h>
#include <Talon.h>

class Claw {
public:
     float SetAngle(float shooterAngle);
     float SetWheel(float wheelSpeed);
     Claw (float clawAnglePort,float clawShooterWheelPort);
     ~Claw();


private:
    Talon *shooterWheel;
    Talon *shooterBase;
    Encoder *wheelEncoder;
    Encoder *shooterEncoder;

};
