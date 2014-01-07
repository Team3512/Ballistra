#include "robot.hpp"
#include <timer.h>
#include <Solenoid.h>

robot::robot() {
	driveFR = new Talon (1);
	driveRR = new Talon (2);
	driveFL = new Talon (3);
	driveRL = new Talon (4);
	driveStick1 = new Joystick (2);
	driveStick2 = new Joystick (1);
	shooterFM = new Victor (5);
	shooterBM = new Victor (6);
	shooter = new Solenoid (2);
	mainCompressor = new Compressor (1,1);
	timer = new Timer ();
	timer2 = new Timer ();
	shooterHeight = new Solenoid (3);
	shifter = new Solenoid (7);
	counter = new Counter (10);
	kinect = new KateKinect();

	
	logger1 = new Logger ();
	ls = new LogStream(logger1);
	consoleSink = new LogConsoleSink();
	logFileSink = new LogFileSink("LogFile.txt");
	logger1 ->addLogSink(consoleSink);
	logger1 ->addLogSink(logFileSink);
	consoleSink->setVerbosityLevels(LogEvent::VERBOSE_ALL);
	logFileSink->setVerbosityLevels(LogEvent::VERBOSE_ALL);
	
	robotDrive = new RobotDrive (driveFR, driveRR, driveFL, driveRL);
}
robot::~robot(){
	delete driveFR;
	delete driveRR;
	delete driveFL;
	delete driveRL;
	delete driveStick1;
	delete driveStick2;
	delete shooterFM;
	delete shooterBM;
	delete shooter;
	delete mainCompressor;
	delete timer;
	delete timer2;
	delete shooterHeight;
	delete shifter;
	delete counter;
	delete kinect;
	delete logger1;
	delete ls;
	delete consoleSink;
	delete logFileSink;
	delete robotDrive;
}
void robot::Autonomous(){
	int i = 0;
	mainCompressor->Start();
	
	while(!timer2->HasPeriodPassed(3.0)){
		robotDrive->ArcadeDrive(1,1,false);
		timer2->Start();
	}
	robotDrive->ArcadeDrive(0,0,false);
	shooterFM->Set(1);
	shooterBM->Set(1);
	while(shooterFM->Get() == true && i<3){
		shooter->Set(1);
		Wait(0.5);
		shooter->Set(0);
		Wait (0.5);
		i++;
	}

}

void robot::OperatorControl() {
	bool b5CurrentlyPressed = false;
	bool b3CurrentlyPressed = false;
	bool b2CurrentlyPressed = true;
	bool triggerLastPressed = false;
	bool triggerCurrentlyPressed = false;
	mainCompressor->Start();
	counter->Start();
	Timer *timer3;
	timer3 = new Timer();
	timer3->Start();
    std::stringstream *ss;
	ss = new std::stringstream();

	
	
	while (IsOperatorControl()){
	 //s = kinect->GetSkeleton();	
	 
	 if(timer3->HasPeriodPassed(1.0)){
		// xDistance = s->GetHandRight().x - s->GetHandLeft().x;
		// yDistance = s->GetHandRight().y - s->GetHandLeft().y;
		 //(*ss) << 60/counter->GetPeriod();
		 (*ls) << SetLogLevel(LogEvent::VERBOSE_INFO) << kinect->GetArmSlope().first << std::flush;
		 (*ls) << SetLogLevel(LogEvent::VERBOSE_INFO) << kinect->GetArmSlope().second << std::flush;
		 //(*ls)<<SetLogLevel(LogEvent::VERBOSE_INFO)<<s->GetHandRight().x<<std::flush;
		 //(*ls) <<SetLogLevel(LogEvent::VERBOSE_LEVEL)<<sqrtf(xDistance*xDistance + yDistance*yDistance);<<std::flush;
		 //logger1->logEvent(LogEvent( ss->str(), LogEvent::VERBOSE_INFO));
		 //ss->str("");
	 }
		 
	
	//arcade drive
	robotDrive->ArcadeDrive(driveStick1->GetX(),driveStick2->GetY(), false);
	//sets shifter
	triggerCurrentlyPressed = driveStick1->GetRawButton(1);
	
	if (triggerCurrentlyPressed == false && triggerLastPressed == true){
		shifter->Set(!shifter->Get());
	}
	triggerLastPressed = triggerCurrentlyPressed;
	
	//shoots frisbee
	if (driveStick2->GetRawButton(1)){
		shooter->Set(true);
		timer->Start();
	}
	
	if (timer->HasPeriodPassed(0.5)){
		shooter->Set(false);
		timer->Stop();
	}
	//sets shoter angle
	if (driveStick2->GetRawButton(3)){
		b3CurrentlyPressed = true;
		b2CurrentlyPressed = false;
	}
	if (b3CurrentlyPressed == true){
		shooterHeight->Set(true);
	}
	if (driveStick2->GetRawButton(2)){
		b2CurrentlyPressed = true;
	}
	if (b2CurrentlyPressed == true){
		shooterHeight->Set(false);
	}
	//sets shooter wheels
	if ( driveStick2->GetRawButton(5) ){
		b5CurrentlyPressed = true;
	}
	if (b5CurrentlyPressed == true){
		shooterFM->Set(driveStick2->GetZ());
		shooterBM->Set(driveStick2->GetZ());
	}
	if ( driveStick2->GetRawButton (4)){
		b5CurrentlyPressed = false;
		shooterFM->Set(0);
		shooterBM->Set(0);
	}
	}
	
}
START_ROBOT_CLASS(robot);
