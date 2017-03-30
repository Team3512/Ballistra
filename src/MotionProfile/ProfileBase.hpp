// Copyright (c) FRC Team 3512, Spartatroniks 2014-2017. All Rights Reserved.

#pragma once

class ProfileBase {
public:
    virtual ~ProfileBase() = default;

    virtual double updateSetpoint(double curSetpoint, double curSource,
                                  double curTime) = 0;
    virtual double setGoal(double goal, double curSource, double t) = 0;
    virtual bool atGoal() = 0;
};
