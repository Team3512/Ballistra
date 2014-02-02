#include "Claw.hpp"

#include <Solenoid.h>

Claw::Claw(float clawRotatePort,float clawWheelPort) :
        m_settings( "RobotSettings.txt" ),
        m_isShooting( false ) {
    m_clawRotator = new GearBox<Talon>( 0 , 3 , 2 , clawRotatePort );
    m_intakeWheel = new GearBox<Talon>( 0 , 0 , 0 , clawWheelPort );

    m_ballShooter.push_back( new Solenoid( 1 ) );
    m_ballShooter.push_back( new Solenoid( 2 ) );
    m_ballShooter.push_back( new Solenoid( 3 ) );
    m_ballShooter.push_back( new Solenoid( 4 ) );
}

Claw::~Claw(){
    // Free solenoids
    for ( unsigned int i = 0 ; i < m_ballShooter.size() ; i++ ) {
        delete m_ballShooter[i];
    }
    m_ballShooter.clear();
}

void Claw::SetAngle(float shooterAngle){
    m_clawRotator->setSetpoint( shooterAngle );
}

double Claw::GetTargetAngle() const {
    return m_clawRotator->getSetpoint();
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

    // Set shooter intake wheel PID
    p = atof( m_settings.getValueFor( "PID_ARM_WHEEL_P" ).c_str() );
    i = atof( m_settings.getValueFor( "PID_ARM_WHEEL_I" ).c_str() );
    d = atof( m_settings.getValueFor( "PID_ARM_WHEEL_D" ).c_str() );
    m_intakeWheel->setPID( p , i , d );
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

void Claw::Update() {
    if ( m_isShooting ) {
        if ( m_shootTimer.HasPeriodPassed( 0.5 ) ) {
            // Return bow to default position
            for ( unsigned int i = 0 ; i < m_ballShooter.size() ; i++ ) {
                m_ballShooter[i]->Set( false );
            }

            // Reset shoot timer
            m_shootTimer.Stop();
            m_shootTimer.Reset();

            // Process is done, allow it to repeat
            m_isShooting = false;
        }
    }
}

bool Claw::IsShooting() const {
    return m_isShooting;
}
