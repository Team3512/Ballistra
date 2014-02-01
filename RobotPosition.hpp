#include <Timer.h>
#include <Encoder.h>
#include <math.h>
class RobotPosition{
public:
    float GetY();
    float GetX();
    float GetPosition();
    float ThetaTotal();
    float deltaT();
    float GetEncoder();
    float GetLeftEncoder();
    void zeroValues();
    RobotPosition(float rightPort1,float rightPort2,float leftPort1,float leftPort2);
    ~RobotPosition();
private:
    Timer *timer3;
    Timer *timer1;
    Timer *timer2;
    Timer *timer4;
    Timer *timer5;
    Encoder *encoder;
    Encoder *leftEncoder;
    float thetaTotal;
    double timerCurrent1;
    double timerLast1;
    double timerCurrent2;
    double timerLast2;
    double timerCurrent3;
    double timerLast3;
    double timerCurrent4;
    double timerLast4;
    double timerCurrent5;
    double timerLast5;
    float y ;
    float x;
    float total;

};
