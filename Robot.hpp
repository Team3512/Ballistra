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
#include "Settings.hpp"

#include "RobotKinect.hpp"
#include "RobotPosition.hpp"

#include "DriverStationDisplay.hpp"
#include "LiveGrapherHost/GraphHost.hpp"
#include "Insight.hpp"

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
    void calibrateTalons();

    void RightLeftAuton();
    void AutonMotionProfile();

    void DS_PrintOut();

private:
    Settings settings;

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

    // Used for sending data to the Driver Station
    DriverStationDisplay<Robot>* driverStation;

    // The LiveGrapher host
    GraphHost pidGraph;

    Insight *insight;

    Logger *logger1;
    LogFileSink *logFileSink;
    LogServerSink *logServerSink;
    LogStream *ls;
};

#endif // _ROBOT_HPP
