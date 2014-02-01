#ifndef _ROBOT_HPP
#define _ROBOT_HPP

#include <SimpleRobot.h>
#include <Talon.h>
#include <RobotDrive.h>
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
#include <Encoder.h>

#include "ADXL345_I2C_ALT.h"

#include "Logging/Logger.h"
#include "Logging/LogConsoleSink.h"
#include "Logging/LogFileSink.h"
#include "Logging/LogServerSink.h"
#include "Logging/LogStream.h"
#include "Logging/SetLogLevel.h"

class robot : public SimpleRobot {
    public:
        robot();
        ~robot();
        void OperatorControl();
        void Autonomous ();
        void Disabled ();
        void Test ();
        bool testDriveTrain(bool shifterState, bool direction, float lowerBound,float upperBound);
        bool testCompressor();


    private:
        Talon *driveFR;
        Talon *driveRR;
        Talon *driveFL;
        Talon *driveRL;
        RobotDrive *robotDrive;
        Joystick *driveStick1;
        Joystick *driveStick2;
        Joystick *shootStick;
        Victor *shooterFM;
        Victor *shooterBM;
        Solenoid *shooter1;
        Solenoid *shooter2;
        Solenoid *shooter3;
        Solenoid *shooter4;
        Compressor *mainCompressor;
        Timer *timer;
        Timer *timer2;
        Timer *timer42;
        Encoder *encoder;
        Counter  *counter;
        Solenoid* shooterArm;
        Solenoid *shifter;
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
