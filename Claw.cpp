#include "Claw.h"

Claw::Claw(float clawAnglePort,float clawShooterWheelPort){
    shooterWheel = new Talon (clawShooterWheelPort);
    shooterBase = new Talon (clawAnglePort);
    wheelEncoder = new Encoder (3,2);
    shooterEncoder = new Encoder(2,3);
}
Claw::~Claw(){
    delete shooterWheel;
    delete shooterBase;
    delete wheelEncoder;
    delete shooterEncoder;
}

float Claw::SetAngle(float shooterAngle){
    shooterEncoder->Start();
    if (shooterAngle < shooterEncoder->Get()){
        shooterBase->Set(0.5);
    }
    if (shooterAngle > shooterEncoder->Get()){
        shooterBase->Set(-0.5);
    }
    if (shooterAngle == shooterEncoder->Get()){
        shooterBase->Set(0.0);
    }
}
