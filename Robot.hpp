#ifndef _ROBOT_HPP
#define _ROBOT_HPP

#include <SimpleRobot.h>
#include <Joystick.h>
#include <Solenoid.h>
#include <Compressor.h>
#include <Timer.h>
#include <DriverStationLCD.h>

#include "Subsystems/DriveTrain.hpp"
#include "Subsystems/Claw.hpp"
#include "ButtonTracker.hpp"

#include "RobotKinect.hpp"
#include "RobotPosition.hpp"
#include "ADXL345_I2C_ALT.h"

#include "Logging/Logger.h"
#include "Logging/LogConsoleSink.h"
#include "Logging/LogFileSink.h"
#include "Logging/LogServerSink.h"
#include "Logging/LogStream.h"
#include "Logging/SetLogLevel.h"

class Robot : public SimpleRobot {
public:
    Robot();
    virtual ~Robot();
    void OperatorControl();
    void Autonomous ();
    void Disabled ();
    void Test ();
    bool testDriveTrain(bool shifterState, bool direction, float lowerBound,float upperBound);
    bool testCompressor();

    void DS_PrintOut();

private:
    DriveTrain *robotDrive;
    Claw *claw;

    Joystick *driveStick1;
    Joystick *driveStick2;
    Joystick *shootStick;

    ButtonTracker drive1Buttons;
    ButtonTracker drive2Buttons;
    ButtonTracker shootButtons;

    Compressor *mainCompressor;

    Timer *autonTimer;
    Timer *displayTimer;

    RobotKinect *kinect;
    RobotPosition *robotPosition;
    ADXL345_I2C_ALT *accelerometer;

    Logger *logger1;
    LogFileSink *logFileSink;
    LogServerSink *logServerSink;
    LogStream *ls;
};

#endif // _ROBOT_HPP
