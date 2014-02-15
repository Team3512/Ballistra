//=============================================================================
//File Name: DriveTrain.cpp
//Description: Provides an interface for this year's drive train
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#include "DriveTrain.hpp"

#include <cmath>
#include <Talon.h>

#define max( x , y ) (((x) > (y)) ? (x) : (y))

const float DriveTrain::maxWheelSpeed = 150.f;

DriveTrain::DriveTrain() :
            TrapezoidProfile( maxWheelSpeed , 5.f ),
            m_settings( "RobotSettings.txt" ) {
    m_squaredInputs = false;
    m_deadband = 0.f;

    m_leftGrbx = new GearBox<Talon>( 7 , 5, 6 , 1 , 2, 3 );
    m_leftGrbx->setReversed( true );

    m_rightGrbx = new GearBox<Talon>( 0 , 3 , 4 , 4 , 5, 6 );

    // c = PI * 10.16cm [wheel diameter]
    // dPerP = c / pulses
    m_leftGrbx->setDistancePerPulse( 3.14159265 * 10.16 / 360.0 );
    m_rightGrbx->setDistancePerPulse( 3.14159265 * 10.16 / 360.0 );

    reloadPID();
}

DriveTrain::~DriveTrain() {
    delete m_leftGrbx;
    delete m_rightGrbx;
}

void DriveTrain::drive( float speed , float turn, float fudgeLeft, float fudgeRight ) {
    // Limit values to [-1 .. 1]
    if ( speed > 1.f ) {
        speed = 1.f;
    }
    else if ( speed < -1.f ) {
        speed = -1.f;
    }
    if ( turn > 1.f ) {
        turn = 1.f;
    }
    else if ( turn < -1.f ) {
        turn = -1.f;
    }

    if ( m_squaredInputs ) {
        if ( turn < 0 ) {
            turn = -pow( turn , 2 );
        }
        else {
            turn = pow( turn , 2 );
        }

        if ( speed < 0 ) {
            speed = -pow( speed , 2 );
        }
        else {
            speed = pow( speed , 2 );
        }
    }

    // Apply joystick deadband
    if ( fabs(speed) < m_deadband ) {
        speed = 0.f;
    }
    if ( fabs(turn) < m_deadband ) {
        turn = 0.f;
    }

    // Will contain left and right speeds
    float wheelSpeeds[2];

    float leftMotorOutput = 0.f;
    float rightMotorOutput = 0.f;

    if ( speed > 0.f ) {
        if ( turn > 0.f ) {
            leftMotorOutput = speed - turn;
            rightMotorOutput = max( speed , turn );
        }
        else {
            leftMotorOutput = max( speed , -turn );
            rightMotorOutput = speed + turn;
        }
    }
    else {
        if ( turn > 0.f ) {
            leftMotorOutput = -max( -speed , turn );
            rightMotorOutput = speed + turn;
        }
        else {
            leftMotorOutput = speed - turn;
            rightMotorOutput = -max( -speed , -turn );
        }
    }

    wheelSpeeds[0] = leftMotorOutput;
    wheelSpeeds[1] = rightMotorOutput;

    normalize( wheelSpeeds , 2 );

    if(speed < 0)
    {
    	float temp;
    	temp = fudgeLeft;
    	fudgeLeft = fudgeRight;
    	fudgeRight = temp;

    }

    m_leftGrbx->setManual( wheelSpeeds[0]*fudgeLeft );
    m_rightGrbx->setManual( wheelSpeeds[1]*fudgeRight );
}

void DriveTrain::squareInputs( bool squared ) {
    m_squaredInputs = squared;
}

void DriveTrain::setDeadband( float band ) {
    m_deadband = band;
}

void DriveTrain::resetEncoders() {
    m_leftGrbx->resetEncoder();
    m_rightGrbx->resetEncoder();
}

void DriveTrain::reloadPID() {
    m_settings.update();

    float p = 0.f;
    float i = 0.f;
    float d = 0.f;

    p = atof( m_settings.getValueFor( "PID_DRIVE_P" ).c_str() );
    i = atof( m_settings.getValueFor( "PID_DRIVE_I" ).c_str() );
    d = atof( m_settings.getValueFor( "PID_DRIVE_D" ).c_str() );

    m_leftGrbx->setPID( p , i , d );
    m_rightGrbx->setPID( p , i , d );
}

void DriveTrain::setLeftSetpoint( double setpt ) {
    m_leftGrbx->setSetpoint( setpt );
}

void DriveTrain::setRightSetpoint( double setpt ) {
    m_rightGrbx->setSetpoint( setpt );
}

void DriveTrain::setLeftManual( float value ) {
    m_leftGrbx->PIDWrite( value );
}

void DriveTrain::setRightManual( float value ) {
    m_rightGrbx->PIDWrite( value );
}

double DriveTrain::getLeftDist() {
    return m_leftGrbx->getDistance();
}

double DriveTrain::getRightDist() {
    return m_rightGrbx->getDistance();
}

double DriveTrain::getLeftRate() {
    return m_leftGrbx->getRate();
}

double DriveTrain::getRightRate() {
    return m_rightGrbx->getRate();
}

double DriveTrain::getLeftSetpoint() {
    return m_leftGrbx->getSetpoint();
}

double DriveTrain::getRightSetpoint() {
    return m_rightGrbx->getSetpoint();
}

void DriveTrain::setGear( bool gear ) {
    m_leftGrbx->setGear( gear );
    m_rightGrbx->setGear( gear );
}

bool DriveTrain::getGear() const {
    return m_leftGrbx->getGear();
}

void DriveTrain::normalize( float* wheelSpeeds , unsigned int arraySize ) {
    double maxMagnitude = fabs( wheelSpeeds[0] );
    unsigned int i;

    for ( i = 1 ; i < arraySize ; i++ ) {
        double temp = fabs( wheelSpeeds[i] );

        if ( maxMagnitude < temp ) {
            maxMagnitude = temp;
        }
    }

    if ( maxMagnitude > 1.f ) {
        for ( i = 0 ; i < arraySize ; i++ ) {
            wheelSpeeds[i] /= maxMagnitude;
        }
    }
}
