#ifndef _ROBOT_HPP
#define _ROBOT_HPP

#include <SimpleRobot.h>
#include <Joystick.h>
#include <Solenoid.h>
#include <Compressor.h>
#include <Timer.h>
#include <Victor.h>
#include <Counter.h>
#include <sstream>
#include "KateKinect.hpp"
#include "RobotPosition.hpp"
#include <DriverStationLCD.h>
#include <math.h>

#include "Subsystems/DriveTrain.hpp"
#include "Subsystems/Claw.hpp"
#include "ButtonTracker.hpp"

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
        Timer *timer2;
        Timer *timer42;
        Timer *displayTimer;
        Counter  *counter;
        Logger *logger1;
        LogConsoleSink *consoleSink;
        LogFileSink *logFileSink;
        LogServerSink *logServerSink;
        LogStream *ls;
        KateKinect *kinect;
        RobotPosition *robotPosition;
        ADXL345_I2C_ALT *accelerometer;
};

#endif

