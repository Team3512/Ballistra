//=============================================================================
//File Name: AutonMotionProfile.cpp
//Description: Drives set distance with motion profiles
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "../Robot.hpp"
#include <Timer.h>

bool Robot::checkReflectiveStrips() {

	return true;
}

void Robot::RightLeftAuton() {
	float targetValue;
	const float targetDistance = 365.0;
	Timer *timer = new Timer;
	timer->Reset();
	robotDrive->resetEncoders();
	timer->Start();
	bool state = checkReflectiveStrips();

	claw->SetAngle(94.0);
	Wait (0.5);

	/* "&& robotDrive->getRightDist() > -5.0" ensures robot doesn't drive
	 * backwards
	 */
	while (robotDrive->getRightDist() < targetDistance && IsEnabled() && robotDrive->getRightDist() > -5.0){
	    targetValue = -0.6 * (1.f - robotDrive->getRightDist() / targetDistance );
	    robotDrive->drive (targetValue , 0);

	    std::cout << "right distance: " << (robotDrive->getRightDist())<< std::endl;
		std::cout << "left distance: " << (robotDrive->getLeftDist()) << std::endl;
		std::cout << "targetValue: " << targetValue << std::endl;

		claw->Update();
		Wait (0.1);
	}

	claw->SetWheelManual(0.0);
	robotDrive->drive(-0.1,0);
	Wait (0.1);
	robotDrive->drive(0,0);
	std::cout << "final right distance: " << (robotDrive->getRightDist()) << std::endl;
	std::cout << "final left distance: " << (robotDrive->getLeftDist()) << std::endl;
	if (state == true){
		claw->Shoot();
	}
	else{
		while (timer->HasPeriodPassed(5) == false){
			Wait (0.01);
		}
		claw->Shoot();
	}
	while (IsEnabled() && IsAutonomous()){
		claw->Update();
		Wait (0.1);
	}
}
