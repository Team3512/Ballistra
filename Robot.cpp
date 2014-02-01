#include "Robot.hpp"

Robot::Robot() :
        drive1Buttons( 1 ),
        drive2Buttons( 2 ),
        shootButtons( 3 ) {
    robotDrive = new DriveTrain();
    claw = new Claw( 5 , 6 );

    driveStick1 = new Joystick (2);
    driveStick2 = new Joystick (1);
    shootStick = new Joystick (3);

    mainCompressor = new Compressor (1,1);
    autonTimer = new Timer ();
    displayTimer = new Timer ();

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
}

Robot::~Robot(){
    delete robotDrive;
    delete claw;

    delete driveStick1;
    delete driveStick2;
    delete shootStick;

    delete mainCompressor;

    delete autonTimer;
    delete displayTimer;

    delete kinect;

    delete logger1;
    delete ls;
    delete logFileSink;
    delete logServerSink;

    delete accelerometer;
}

void Robot::Autonomous(){
#if 0
    mainCompressor->Start();
    autonTimer->Start();

    while(!autonTimer->HasPeriodPassed(3.0)){
        DS_PrintOut();

        robotDrive->drive(1,1);
    }

    robotDrive->drive(0,0);
#endif
}

void Robot::Disabled(){
    while (IsDisabled()){
        DS_PrintOut();

        logServerSink->acceptor(false);
        Wait (0.1);
    }
}

void Robot::Test(){
    mainCompressor->Start();

    testDriveTrain(true, true, -1, 1);
    testDriveTrain(true,false, -1, 1);
    testDriveTrain(false, true, -1, 1);
    testDriveTrain(false, false, -1, 1);
    testCompressor();
    robotDrive->drive( 0 , 0 );
}

bool Robot::testCompressor(){
    if (mainCompressor->GetPressureSwitchValue() == 0){
        return false;
    }
    else{
        return true;
    }
}

bool Robot::testDriveTrain(bool shifterState, bool direction, float lowerBound,float upperBound){
    Timer timer;
    timer.Start();

    // Converts direction (1 or 0) to 1 or -1 respectively
    int i = static_cast<int>(direction) * 2 - 1;

    robotDrive->setGear( shifterState );

    while (!timer.HasPeriodPassed(3.0)){
        robotDrive->drive( i , 0 );
        Wait (0.1);
        if ( !(lowerBound < robotDrive->getLeftRate() && robotDrive->getLeftRate() < upperBound) ) {
            return false;
        }
    }

    return true;
}

void Robot::OperatorControl() {
    mainCompressor->Start();
    robotPosition->zeroValues();

    while (IsOperatorControl() && IsEnabled()){
        DS_PrintOut();

        //Kinect Drive
        //robotDrive->setLeftManual( kinect->GetArmScale().second );
        //robotDrive->setRightManual( kinect->GetArmScale().first );

        //arcade Drive
        robotDrive->drive( driveStick1->GetY() , driveStick2->GetX() );

        if ( drive1Buttons.releasedButton( 1 ) ) {
            robotDrive->setGear( !robotDrive->getGear() );
        }

        //Shoots Ball
        if (shootStick->GetRawButton(1) && !claw->IsShooting()){
            claw->Shoot();
        }

        claw->Update();

        drive1Buttons.updateButtons();
        drive2Buttons.updateButtons();
    }
}

void Robot::DS_PrintOut() {
    if(displayTimer->HasPeriodPassed(0.05)){
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
}

START_ROBOT_CLASS(Robot);

