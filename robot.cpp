#include "robot.hpp"

robot::robot() {
	driveFL = new Talon (1);
	driveRL = new Talon (2);
	driveFR = new Talon (3);
	driveRR = new Talon (4);

	driveStick1 = new Joystick (2);
	driveStick2 = new Joystick (1);
	shootStick = new Joystick (3);
	shooterFM = new Victor (5);
	shooterBM = new Victor (6);
	shooter1 = new Solenoid (100);
	shooter2 = new Solenoid (100);
	shooter3 = new Solenoid (100);
	shooter4 = new Solenoid (100);
	mainCompressor = new Compressor (1,1);
	timer = new Timer ();
	timer2 = new Timer ();
	timer42 = new Timer ();
	encoder = new Encoder(8,7);
	shooterArm = new Solenoid (3);
	shifter = new Solenoid (7);
	kinect = new KateKinect();
	robotPosition = new RobotPosition(1,2,3,4);
	accelerometer = new ADXL345_I2C_ALT (1);


	logger1 = new Logger ();
	ls = new LogStream(logger1);
	logFileSink = new LogFileSink("LogFile.txt");
	logServerSink = new LogServerSink();
	logger1 ->addLogSink(logFileSink);
	logger1->addLogSink(logServerSink);
	logFileSink->setVerbosityLevels(LogEvent::VERBOSE_ALL);
	logServerSink->setVerbosityLevels(LogEvent::VERBOSE_ALL);
	logServerSink->startServer (4097);

	robotDrive = new RobotDrive (driveFL, driveRL, driveFR, driveRR);
}
robot::~robot(){
	delete driveFR;
	delete driveRR;
	delete driveFL;
	delete driveRL;
	delete   driveStick1;
	delete driveStick2;
	delete shootStick;
	delete shooterFM;
	delete shooterBM;
	delete mainCompressor;
	delete timer;
	delete timer2;
	delete timer42;
	delete shooterArm;
	delete shifter;
	delete counter;
	delete kinect;
	delete logger1;
	delete ls;
	delete consoleSink;
	delete logFileSink;
	delete logServerSink;
	delete robotDrive;
	delete accelerometer;
}

void robot::Autonomous(){
	/*int i = 0;
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
	}*/


}
void robot::Disabled(){
	while (IsDisabled()){
		 logServerSink->acceptor(false);
		 Wait (1.0);
	}
}

void robot::Test(){
	mainCompressor->Start();
	encoder->Start();
	encoder->SetDistancePerPulse(60.0/360);


	testDriveTrain(true, true, -1, 1);
	testDriveTrain(true,false, -1, 1);
	testDriveTrain(false, true, -1, 1);
	testDriveTrain(false, false, -1, 1);
	testCompressor();
	robotDrive->ArcadeDrive(0,0,false);
}
bool robot::testCompressor(){
	Timer *timer;
	timer = new Timer();
	timer->Start();
	if (mainCompressor->GetPressureSwitchValue() == 0 && timer->Get()<10.0){
		return false;
	}
	else{
		return true;
	}

}

bool robot::testDriveTrain(bool shifterState, bool direction, float lowerBound,float upperBound){
	timer42->Start();
	timer42->Reset();
	int i;
	i = 0;


	if (shifterState == true){
		shifter->Set(true);
	}
	else{
		shifter->Set(false);
	}
	if (direction == true){
		i = 1;
	}
	else{
		i = -1;
	}
	while (!timer42->HasPeriodPassed(3.0)){
		robotDrive->ArcadeDrive(i, 0, false);
		Wait (0.1);
		if(!(lowerBound<encoder->GetRate() && encoder->GetRate()<upperBound)){
			return false;
		}


	}
	return true;

}

void robot::OperatorControl() {
	//bool b5CurrentlyPressed = false;
	bool b3CurrentlyPressed = false;
	bool b2CurrentlyPressed = true;
	bool triggerLastPressed = false;
	bool triggerCurrentlyPressed = false;
	mainCompressor->Start();
	Timer *timer3;
	timer3 = new Timer();
	timer3->Reset();
	timer3->Start();
    //std::stringstream *ss;
	//ss = new std::stringstream();
	robotPosition->zeroValues();

	while (IsOperatorControl() && IsEnabled()){

		if(timer3->HasPeriodPassed(0.05)){
		 //(*ls) << SetLogLevel(LogEvent::VERBOSE_INFO) << kinect->GetArmScale().second << std::flush;
		 //logServerSink->acceptor(false);

		 DriverStationLCD *userMessages = DriverStationLCD::GetInstance();
		 userMessages->Clear();

		 userMessages->Printf(DriverStationLCD::kUser_Line1, 1,"accelerometer %f ",accelerometer->GetAcceleration(ADXL345_I2C_ALT::kAxis_X));

		 /*userMessages->Printf(DriverStationLCD::kUser_Line2, 1,"Encoder2: %f",robotPosition->GetLeftEncoder());
		 userMessages->Printf(DriverStationLCD::kUser_Line3, 1," x: %f", robotPosition->GetX() );
		 userMessages->Printf(DriverStationLCD::kUser_Line4, 1," y: %f", robotPosition->GetY() );*/

		 userMessages->UpdateLCD();
		}

		//Kinect Drive
		//robotDrive->TankDrive(kinect->GetArmScale().second,kinect->GetArmScale().first);

		//arcade Drive
		robotDrive->ArcadeDrive(driveStick1->GetY(),driveStick2->GetX(), false);

		//Set Shifter
		triggerCurrentlyPressed = driveStick1->GetRawButton(1);

		if (triggerCurrentlyPressed == false && triggerLastPressed == true){
			shifter->Set(!shifter->Get());
		}
		triggerLastPressed = triggerCurrentlyPressed;

		//Shoots Ball
		if (shootStick->GetRawButton(1)){
			shooter1->Set(true);
			shooter2->Set(true);
			shooter3->Set(true);
			shooter4->Set(true);

			timer->Start();
		}

		if (timer->HasPeriodPassed(0.5)){
			shooter1->Set(false);
			shooter2->Set(false);
			shooter3->Set(false);
			shooter4->Set(false);
			timer->Stop();
		}
		//Sets Shooter Arm
		if (shootStick->GetRawButton(3)){
			b3CurrentlyPressed = true;
			b2CurrentlyPressed = false;
		}
		if (b3CurrentlyPressed == true){
			//shooterHeight->Set(true);
		}
		if (shootStick->GetRawButton(2)){
			b2CurrentlyPressed = true;
		}
		if (b2CurrentlyPressed == true){
			//shooterHeight->Set(false);
		}
		/*Sets Shooter Wheels
		if ( shootStick->GetRawButton(5) ){
			b5CurrentlyPressed = true;
		}
		if (b5CurrentlyPressed == true){
			shooterFM->Set(driveStick2->GetZ());
			shooterBM->Set(driveStick2->GetZ());
		}
		if ( shootStick->GetRawButton (4)){
			b5CurrentlyPressed = false;
			shooterFM->Set(0);
			shooterBM->Set(0);
		}*/
	}
}


START_ROBOT_CLASS(robot);
