#ifndef _ROBOT_HPP
#define _ROBOT_HPP

#include <Talon.h>
#include <RobotDrive.h>
#include <Joystick.h>
#include <Solenoid.h>
#include <Compressor.h>
#include <Timer.h>
#include <Victor.h>
#include <SimpleRobot.h>
#include <Counter.h>
#include <sstream>
#include "KateKinect.hpp"

#include "Logging/Logger.h"
#include "Logging/LogConsoleSink.h"
#include "Logging/LogFileSink.h"
#include "Logging/LogStream.h"
#include "Logging/SetLogLevel.h"

class robot : public SimpleRobot {
	public:
		robot();
		~robot();
		void OperatorControl();
		void Autonomous ();
	
	private:
		Talon *driveFR;
	    Talon *driveRR;
	    Talon *driveFL;
	    Talon *driveRL;
	    RobotDrive *robotDrive;
	    Joystick *driveStick1;
	    Joystick *driveStick2;
	    Victor *shooterFM;
	    Victor *shooterBM;
	    Solenoid *shooter;
	    Compressor *mainCompressor;
	    Timer *timer;
	    Timer *timer2;
	    Counter  *counter;
	    Solenoid* shooterHeight;
	    Solenoid *shifter;
	    Logger *logger1;
	    LogConsoleSink *consoleSink;
	    LogFileSink *logFileSink;
	    LogStream *ls;
	    KateKinect *kinect;
	    };
#endif
