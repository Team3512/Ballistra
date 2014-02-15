#include "Claw.hpp"

#include <Solenoid.h>
#include <DriverStationLCD.h>

Claw::Claw(float clawRotatePort,float clawWheelPort) :
        m_settings( "RobotSettings.txt" ),
        m_isShooting( false ) {
    m_clawRotator = new GearBox<Talon>( 0 , 7 , 8 , clawRotatePort );
    m_intakeWheel = new GearBox<Talon>( 0 , 0 , 0 , clawWheelPort );

    // Sets degrees rotated per pulse of encoder
    m_clawRotator->setDistancePerPulse( (1.0/71.0f)*14.0 /44.0 );

    m_ballShooter.push_back( new
    Solenoid( 1 ) );
    m_ballShooter.push_back( new Solenoid( 2 ) );
    m_ballShooter.push_back( new Solenoid( 3 ) );
    m_ballShooter.push_back( new Solenoid( 4 ) );
    collectorArm = new Solenoid(5);
    vacuum = new Solenoid (6);
    m_isVacuuming = false;
}

Claw::~Claw(){
    // Free solenoids
    for ( unsigned int i = 0 ; i < m_ballShooter.size() ; i++ ) {
        delete m_ballShooter[i];
        delete collectorArm;
    }
    m_ballShooter.clear();
}

void Claw::SetAngle(float shooterAngle){
    m_clawRotator->setSetpoint( shooterAngle );
}

void Claw::ManualSetAngle(float value) {
	m_clawRotator->setManual(value);
}

double Claw::GetTargetAngle() const {
    return m_clawRotator->getSetpoint();
}

double Claw::getDistance()
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
    if ( !m_isShooting ) {
        for ( unsigned int i = 0 ; i < m_ballShooter.size() ; i++ ) {
            m_ballShooter[i]->Set( true );
        }

        m_shootTimer.Start();

        m_isShooting = true;
    }
}

void Claw::SetCollectorMode(bool collectorMode){
	if(collectorMode == true){
		collectorArm->Set(true);
	}
	else{
		collectorArm->Set(false);
	}
}
bool Claw::GetCollectorMode(){
	return collectorArm->Get();
}

void Claw::Update() {
    if ( m_isShooting ) {
        if ( m_shootTimer.HasPeriodPassed( 1.0 ) ) {
            // Return bow to default position
            for ( unsigned int i = 0 ; i < m_ballShooter.size() ; i++ ) {
                m_ballShooter[i]->Set( false );
            }
            collectorArm->Set(false);
            // Engage vacuum
            m_isVacuuming = true;
            vacuum->Set(true);
            vacuumTimer.Start();

            // Reset shoot timer
            m_shootTimer.Stop();
            m_shootTimer.Reset();

            // Process is done, allow it to repeat
            m_isShooting = false;
        }
    }
    if (m_isVacuuming){
        if (vacuumTimer.HasPeriodPassed(1.5)){
        	vacuum->Set(false);
        	m_isVacuuming = false;

        	vacuumTimer.Stop();
        	vacuumTimer.Reset();
        }
    }


    DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line1, "Distance:  %f", m_clawRotator->getDistance());
    DriverStationLCD::GetInstance()->PrintfLine(DriverStationLCD::kUser_Line2, "Rate:  %f", m_clawRotator->getRate());
    DriverStationLCD::GetInstance()->UpdateLCD();

}

bool Claw::IsShooting() const {
    return m_isShooting;
}
