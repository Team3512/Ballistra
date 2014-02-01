#include "Robot.hpp"

Robot::Robot() :
        drive1Buttons( 1 ),
        drive2Buttons( 2 ),
        shootButtons( 3 ) {
    driveStick1 = new Joystick (2);
    driveStick2 = new Joystick (1);
    shootStick = new Joystick (3);
    mainCompressor = new Compressor (1,1);
    timer2 = new Timer ();
    timer42 = new Timer ();
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

    robotDrive = new DriveTrain();
    claw = new Claw( 5 , 6 );
}

Robot::~Robot(){
    delete driveStick1;
    delete driveStick2;
    delete shootStick;
    delete mainCompressor;
    delete timer2;
    delete timer42;
    delete displayTimer;
    delete counter;
    delete kinect;
    delete logger1;
    delete ls;
    delete consoleSink;
    delete logFileSink;
    delete logServerSink;
    delete robotDrive;
    delete claw;
    delete accelerometer;
}

void Robot::Autonomous(){
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

void Robot::Disabled(){
    while (IsDisabled()){
         logServerSink->acceptor(false);
         Wait (1.0);
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
    // Converts direction (1 or 0) to 1 or -1 respectively
    int i = static_cast<int>(direction) * 2 - 1;

    robotDrive->setGear( shifterState );

    while (!timer42->HasPeriodPassed(3.0)){
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


START_ROBOT_CLASS(Robot);

