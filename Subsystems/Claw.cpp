#include "Claw.hpp"

Claw::Claw(float clawAnglePort,float clawShooterWheelPort) :
        m_shooterWheel( clawShooterWheelPort ),
        m_shooterBase( clawAnglePort ),
        m_wheelEncoder( 3 , 2 ),
        m_shooterEncoder( 2 , 3 ),
        m_isShooting( false ) {
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
    m_shooterEncoder.Start();

    if (shooterAngle < m_shooterEncoder.Get()){
        m_shooterBase.Set(0.5);
    }
    else if (shooterAngle > m_shooterEncoder.Get()){
        m_shooterBase.Set(-0.5);
    }
    else {
        m_shooterBase.Set(0.0);
    }
}

void Claw::SetWheel(float wheelSpeed) {

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
