#include "Claw.hpp"

#ifndef M_PI
#define M_PI 3.14159265
#endif

#include <Solenoid.h>
#include <DriverStationLCD.h>

Claw::Claw(unsigned int clawRotatePort, unsigned int clawWheelPort, unsigned int zeroSwitchPort) :
        m_settings( "RobotSettings.txt" )
        {
    m_clawRotator = new GearBox<Talon>( 0 , 7 , 8 , clawRotatePort );
    m_intakeWheel = new GearBox<Talon>( 0 , 0 , 0 , clawWheelPort );

    // Sets degrees rotated per pulse of encoder
    m_clawRotator->setDistancePerPulse( (1.0/71.0f)*14.0 /44.0 );
    m_clawRotator->setReversed(true);

    m_ballShooter.push_back( new Solenoid( 1 ) );
    m_ballShooter.push_back( new Solenoid( 2 ) );
    m_ballShooter.push_back( new Solenoid( 3 ) );
    m_ballShooter.push_back( new Solenoid( 4 ) );

    m_zeroSwitch = new DigitalInput(zeroSwitchPort);

    //magical values found using empirical testing don't change.
    setK(0.238f);
    m_l = 69.0f;

    m_collectorArm = new Solenoid(5);
    m_vacuum = new Solenoid (6);

    ReloadPID();
    m_shooterStates = SHOOTER_IDLE;
}

Claw::~Claw(){
    // Free solenoids
    for ( unsigned int i = 0 ; i < m_ballShooter.size() ; i++ ) {
        delete m_ballShooter[i];
    }

    delete m_collectorArm;
    delete m_zeroSwitch;
    delete m_vacuum;
    m_ballShooter.clear();
}

void Claw::SetAngle(float shooterAngle){
    m_clawRotator->setSetpoint( shooterAngle );
    m_setpoint = shooterAngle;
}

void Claw::ManualSetAngle(float value) {
	if((!m_zeroSwitch->Get() && value > 0) || m_zeroSwitch->Get())
	{
		m_clawRotator->setManual(value);

	}

}

double Claw::GetTargetAngle() const {
    return m_clawRotator->getSetpoint();
}

double Claw::GetAngle()
{
	return m_clawRotator->getDistance();

}

void Claw::SetWheelSetpoint( float speed ) {
    m_intakeWheel->setSetpoint( speed );
}

void Claw::SetWheelManual( float speed ) {
    m_intakeWheel->setManual( speed );
}

void Claw::ResetEncoders() {
    m_clawRotator->resetEncoder();
    m_intakeWheel->resetEncoder();
}

void Claw::ReloadPID() {
    m_settings.update();

    float p = 0.f;
    float i = 0.f;
    float d = 0.f;

    // Set shooter rotator PID
    p = atof( m_settings.getValueFor( "PID_ARM_ROTATE_P" ).c_str() );
    i = atof( m_settings.getValueFor( "PID_ARM_ROTATE_I" ).c_str() );
    d = atof( m_settings.getValueFor( "PID_ARM_ROTATE_D" ).c_str() );
    m_clawRotator->setPID( p , i , d );
}

void Claw::Shoot() {
	if (m_shooterStates == SHOOTER_IDLE){
		m_collectorArm->Set(true);
		m_shooterStates = SHOOTER_ARMISLIFTING;
		m_shootTimer.Start();
		m_shootTimer.Reset();
	}

}

void Claw::SetCollectorMode(bool collectorMode){
	if(collectorMode == true){
		m_collectorArm->Set(true);
	}
	else{
		m_collectorArm->Set(false);
	}
}
bool Claw::GetCollectorMode(){
	return m_collectorArm->Get();
}

void Claw::Update() {
	if (m_shooterStates == SHOOTER_ARMISLIFTING && m_shootTimer.HasPeriodPassed(0.5)){
		for ( unsigned int i = 0 ; i < m_ballShooter.size() ; i++ ) {
		    m_ballShooter[i]->Set( true );
		}
		m_shootTimer.Reset();
		m_shooterStates = SHOOTER_SHOOTING;
	}
	if (m_shooterStates == SHOOTER_SHOOTING && m_shootTimer.HasPeriodPassed(1.0)){
		for ( unsigned int i = 0 ; i < m_ballShooter.size() ; i++ ) {
		     m_ballShooter[i]->Set( false );
		}
		m_vacuum->Set(true);
		m_shootTimer.Reset();
		m_shooterStates = SHOOTER_VACUUMING;
	}
	if (m_shooterStates == SHOOTER_VACUUMING && m_shootTimer.HasPeriodPassed(1.5)){
		m_vacuum->Set(false);
		m_collectorArm->Set (false);

		m_shootTimer.Reset();
		m_shooterStates = SHOOTER_IDLE;
	}
	setF(calcF());


	if(!m_zeroSwitch->Get())
	{
		m_clawRotator->resetPID();
		ResetEncoders();

	}

	//fixes the reset not fully touching zeroSwitch because of gradual encoder error
	if(m_zeroSwitch->Get() && GetTargetAngle() <= 0 && m_clawRotator->onTarget())
	{
		m_clawRotator->setSetpoint(GetTargetAngle()-0.5f);

	}
	else if(!m_zeroSwitch->Get() && GetTargetAngle() <= 0 && m_clawRotator->onTarget())
	{
		m_clawRotator->setSetpoint(0);

	}

    DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Distance:  %f", m_clawRotator->getDistance());
    DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line2, "Rate:  %f", m_clawRotator->getRate());
    DriverStationLCD::GetInstance()->UpdateLCD();

}

void Claw::setF(float f)
{
	m_clawRotator->setF(f);

}

void Claw::setK(float k)
{
	m_k = k;

}

float Claw::calcF()
{
	if(GetTargetAngle() == 0)
	{
		return 0.0f;

	}

	return m_k*cos((GetAngle()+m_l)*M_PI/180.0f)/GetTargetAngle()*M_PI/180.0f;

}

bool Claw::IsShooting() const {
    if (m_shooterStates != SHOOTER_IDLE){
    	return true;
    }
    else{
    	return false;
    }
}
