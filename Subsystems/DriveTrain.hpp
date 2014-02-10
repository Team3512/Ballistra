//=============================================================================
//File Name: DriveTrain.hpp
//Description: Provides an interface for this year's drive train
//Author: FRC Team 3512, Spartatroniks
//=============================================================================

#ifndef DRIVE_TRAIN_HPP
#define DRIVE_TRAIN_HPP

#include "../Settings.hpp"
#include "../MotionProfile/TrapezoidProfile.hpp"
#include "GearBox.hpp"

#include <Talon.h>

class DriveTrain : public TrapezoidProfile {
public:
    DriveTrain();
    virtual ~DriveTrain();

    /* Drives robot with given speed and turn values [-1..1].
     * This is a convenience function for use in Operator Control.
     */
    void drive( float speed , float turn, float fudgeLeft = 1, float fudgeRight = 1 );

    /* If enabled, squares joystick inputs for fine-tuned driving at low speeds
     * while maintaining max speed
     */
    void squareInputs( bool squared );

    // Sets joystick deadband
    void setDeadband( float band );

    // Set encoder distances to 0
    void resetEncoders();

    // Reload PID constants
    void reloadPID();

    // Set wheel setpoints (see GearBox::setSetpoint(float))
    void setLeftSetpoint( double setpt );
    void setRightSetpoint( double setpt );

    // Directly set wheel speeds [0..1] (see GearBox::setManual(float))
    void setLeftManual( float value );
    void setRightManual( float value );

    // Returns encoder distances
    double getLeftDist();
    double getRightDist();

    // Returns encoder rates
    double getLeftRate();
    double getRightRate();

    // Returns encoder PID loop setpoints
    double getLeftSetpoint();
    double getRightSetpoint();

    // Shifts internal gearboxes
    void setGear( bool gear );

    // Returns gear of internal gearboxes
    bool getGear() const;

    const static float maxWheelSpeed;

private:
    Settings m_settings;

    bool m_squaredInputs;
    float m_deadband;

    GearBox<Talon>* m_leftGrbx;
    GearBox<Talon>* m_rightGrbx;

    // Normalize motor values if any are outside of [-1..1]
    void normalize( float* wheelSpeeds , unsigned int arraySize );
};

#endif // DRIVE_TRAIN_HPP
