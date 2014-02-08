#include "Robot.hpp"
#include <cmath>

Robot::Robot() :
        settings( "RobotSettings.txt" ),
        drive1Buttons( 1 ),
        drive2Buttons( 2 ),
        shootButtons( 3 ),
        pidGraph( 3513 ) {
    robotDrive = new DriveTrain();
    claw = new Claw( 8, 7 );

    driveStick1 = new Joystick (1);
    driveStick2 = new Joystick (2);
    shootStick = new Joystick (3);

    mainCompressor = new Compressor (1,1);
    autonTimer = new Timer ();
    displayTimer = new Timer ();

    kinect = new RobotKinect();
    //robotPosition = new RobotPosition(1,2,3,4);
    accelerometer = new ADXL345_I2C_ALT (1);

    driverStation = DriverStationDisplay<Robot>::getInstance( atoi( settings.getValueFor( "DS_Port" ).c_str() ) );

    driverStation->addAutonMethod( "MotionProfile" , &Robot::AutonMotionProfile , this );

    pidGraph.resetTime();
    pidGraph.setSendInterval( 200 );

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

void Robot::OperatorControl() {
    mainCompressor->Start();
    //robotPosition->zeroValues();

    while (IsOperatorControl() && IsEnabled()){
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

        //Engage collector
        if( drive1Buttons.releasedButton(2)) {
        	claw->SetCollectorMode(!claw->GetCollectorMode());
        }

        //claw->SetAngle(34);

        if(shootStick->GetRawButton(12))
        {
        	claw->ResetEncoders();

        }

        if(shootStick->GetRawButton(6))
        {
        	claw->SetAngle(90);

        }
        else if(shootStick->GetRawButton(7))
        {
        	claw->SetAngle(180);

        }
        else
        {
        	claw->ManualSetAngle(shootStick->GetY()/1.2);

        }

        claw->Update();
        if (pidGraph.hasIntervalPassed()){
        	pidGraph.graphData(claw->GetTargetAngle(),"Left Setpoint");
        	pidGraph.graphData(claw->getDistance(), "Left PID");
            pidGraph.resetInterval();

        }

        if ( drive1Buttons.releasedButton( 8 ) ) {
            robotDrive->reloadPID();
            claw->ReloadPID();
        }

        drive1Buttons.updateButtons();
        drive2Buttons.updateButtons();
    }
}

void Robot::Autonomous(){
    autonTimer->Reset();
    autonTimer->Start();

    driverStation->execAutonomous();

    autonTimer->Stop();
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

void Robot::DS_PrintOut() {
    if ( pidGraph.hasIntervalPassed() ) {
        pidGraph.graphData( robotDrive->getLeftDist() , "Left PID" );
        pidGraph.graphData( robotDrive->getLeftSetpoint() , "Left Setpoint" );

        pidGraph.resetInterval();
    }

    if(displayTimer->HasPeriodPassed(0.1)){
        //(*ls) << SetLogLevel(LogEvent::VERBOSE_INFO) << kinect->GetArmScale().second << std::flush;
        //logServerSink->acceptor(false);

        DriverStationLCD *userMessages = DriverStationLCD::GetInstance();
        userMessages->Clear();

        userMessages->Printf(DriverStationLCD::kUser_Line1, 1,"accelerometer %f ",accelerometer->GetAcceleration(ADXL345_I2C_ALT::kAxis_X));

        /*userMessages->Printf(DriverStationLCD::kUser_Line2, 1,"Encoder2: %f",robotPosition->GetLeftEncoder());
         userMessages->Printf(DriverStationLCD::kUser_Line3, 1," x: %f", robotPosition->GetX() );
         userMessages->Printf(DriverStationLCD::kUser_Line4, 1," y: %f", robotPosition->GetY() );*/

        userMessages->UpdateLCD();

        driverStation->clear();

        DS::AddElementData( driverStation , "LEFT_RPM" , robotDrive->getLeftRate() );
        DS::AddElementData( driverStation , "RIGHT_RPM" , robotDrive->getRightRate() );
        DS::AddElementData( driverStation , "LEFT_DIST" , robotDrive->getLeftDist() );
        DS::AddElementData( driverStation , "RIGHT_DIST" , robotDrive->getRightDist() );

        driverStation->sendToDS();
    }

    driverStation->receiveFromDS();
}

START_ROBOT_CLASS(Robot);
