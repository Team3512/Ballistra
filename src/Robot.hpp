// Copyright (c) FRC Team 3512, Spartatroniks 2014-2017. All Rights Reserved.

#pragma once

#include <Compressor.h>
#include <DriverStationLCD.h>
#include <Joystick.h>
#include <SimpleRobot.h>
#include <Solenoid.h>
#include <Timer.h>

#include "ButtonTracker.hpp"
#include "DriverStationDisplay.hpp"
#include "LiveGrapherHost/GraphHost.hpp"
#include "Logging/LogConsoleSink.h"
#include "Logging/LogFileSink.h"
#include "Logging/LogServerSink.h"
#include "Logging/LogStream.h"
#include "Logging/Logger.h"
#include "Logging/SetLogLevel.h"
#include "RobotKinect.hpp"
#include "Settings.hpp"
#include "Subsystems/Claw.hpp"
#include "Subsystems/DriveTrain.hpp"

class Robot : public SimpleRobot {
public:
    Robot();
    virtual ~Robot();
    void OperatorControl();
    void Autonomous();
    void Disabled();
    void Test();
    bool testDriveTrain(bool shifterState, bool direction, float lowerBound,
                        float upperBound);
    bool testCompressor();
    void calibrateTalons();

    bool checkReflectiveStrips();

    void RightLeftAuton();
    void AutonMotionProfile();
    void DriveForwardAuton();
    void SideAuton();
    void NoopAuton();

    void DS_PrintOut();

private:
    Settings settings;

    DriveTrain* robotDrive;
    Claw* claw;

    Joystick* driveStick1;
    Joystick* driveStick2;
    Joystick* shootStick;

    ButtonTracker drive1Buttons;
    ButtonTracker drive2Buttons;
    ButtonTracker shootButtons;

    Compressor* mainCompressor;

    Timer* autonTimer;
    Timer* displayTimer;

    RobotKinect* kinect;

    // Used for sending data to the Driver Station
    DriverStationDisplay<Robot>* driverStation;

    // The LiveGrapher host
    GraphHost pidGraph;

    Logger* logger1;
    LogFileSink* logFileSink;
    LogServerSink* logServerSink;
    LogStream* ls;
};
