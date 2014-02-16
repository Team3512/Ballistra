//=============================================================================
//File Name: AutonMotionProfile.cpp
//Description: Drives set distance with motion profiles
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "../Robot.hpp"
#include <Timer.h>

bool Robot::checkReflectiveStrips() {

	return false;
}

void Robot::RightLeftAuton() {
	float targetValue;
	const float targetDistance = 335.0;
	Timer *timer = new Timer;
	timer->Reset();
	robotDrive->resetEncoders();
	timer->Start();
	bool state = checkReflectiveStrips();
	claw->SetAngle(175);
	while ((robotDrive->getRightDist())*-1<targetDistance && IsEnabled()){
		targetValue = ((targetDistance - (robotDrive->getRightDist()*-1))/targetDistance)*-1;
		std::cout << "right distance: " << (robotDrive->getRightDist())*-1 << std::endl;
		std::cout << "left distance: " << (robotDrive->getLeftDist())*-1 << std::endl;
		std::cout << "targetValue: " << targetValue << std::endl;
		robotDrive->drive (targetValue , 0);
		Wait (0.1);
	}
	robotDrive->drive(0,0);
	std::cout << "final right distance: " << (robotDrive->getRightDist())*-1 << std::endl;
	std::cout << "final left distance: " << (robotDrive->getLeftDist())*-1 << std::endl;
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
	}
}
