#include "RobotPosition.hpp"

RobotPosition::RobotPosition(float rightPort1,float rightPort2,float leftPort1,float leftPort2){
    timer3 = new Timer();
    timer3->Start();
    timer1 = new Timer();
    timer1->Start();
    timer2 = new Timer ();
    timer2->Start();
    timer4 = new Timer();
    timer4->Start();
    timer5 = new Timer();
    timer5->Start();
    encoder = new Encoder (rightPort1, rightPort2, true);
    leftEncoder = new Encoder(leftPort1, leftPort2, true);
    encoder->Start();
    leftEncoder->Start();
    thetaTotal = 0;
    timerCurrent1 = 0;
    timerLast1 = 0;
    timerCurrent2 = 0;
    timerLast2 = 0;
    timerCurrent3 = 0;
    timerLast3 = 0;
    timerCurrent4 = 0;
    timerLast4 = 0;
    timerCurrent5 = 0;
    timerLast5 = 0;
    y = 0;
    x = 0;
    total = 0;
    encoder->SetDistancePerPulse((3.14*4.0)/360);
    leftEncoder->SetDistancePerPulse((3.14*4.0)/360);

}
RobotPosition::~RobotPosition(){
    delete timer4;
    delete timer1;
    delete timer2;
    delete timer3;
    delete timer5;
    delete encoder;
    delete leftEncoder;

}
float RobotPosition::deltaT(){
    timerCurrent1 = timer1->Get();
    float temp = timerCurrent1 - timerLast1;
    timerLast1 = timerCurrent1;
    return temp;

}
void RobotPosition::zeroValues(){
    encoder->Reset();
    leftEncoder->Reset();
    thetaTotal = 0;
    timerCurrent1 = 0;
    timerLast1 = 0;
    timerCurrent2 = 0;
    timerLast2 = 0;
    timerCurrent3 = 0;
    timerLast3 = 0;
    timerCurrent4 = 0;
    timerLast4 = 0;
    timerCurrent5 = 0;
    timerLast5 = 0;
    y = 0;
    x = 0;
    total = 0;
    timer1->Reset();
    timer2->Reset();
    timer3->Reset();
    timer4->Reset();
    timer5->Reset();
}

float RobotPosition::GetEncoder(){
    return encoder->GetRate();
}

float RobotPosition::GetLeftEncoder(){
    return leftEncoder->GetRate();
}

float RobotPosition::ThetaTotal(){
    timerCurrent5 = timer5->Get();
    thetaTotal = thetaTotal + ((encoder->GetRate()*-1)-leftEncoder->GetRate())*(timerCurrent5-timerLast5)/35.6;
    timerLast5 = timerCurrent5;
    return thetaTotal;
}

float RobotPosition::GetPosition(){
    timerCurrent2 = timer2->Get();
    total = total + encoder->GetRate()* (timerCurrent2 - timerLast2);
    timerLast2 = timerCurrent2;
    return total;
}

float RobotPosition::GetY(){
    timerCurrent3 = timer3->Get();
    ThetaTotal();
    //Wait (.01);
    y = y + (((encoder->GetRate()*-1)+(leftEncoder->GetRate())/2)*sin(thetaTotal))*(timerCurrent3-timerLast3);
    timerLast3 = timerCurrent3;
    return y;

}

float RobotPosition::GetX(){
    timerCurrent4 = timer4->Get();
    ThetaTotal();
    //Wait(.01);
    x = x + (((encoder->GetRate()*-1)+(leftEncoder->GetRate())/2)*cos(thetaTotal))*(timerCurrent4-timerLast4);
    timerLast4 = timerCurrent4;
    return x;

}
