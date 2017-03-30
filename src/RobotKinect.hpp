// Copyright (c) FRC Team 3512, Spartatroniks 2014-2017. All Rights Reserved.

#pragma once

#include <utility>

#include <Kinect.h>

class RobotKinect {
public:
    std::pair<float, float> GetArmScale();
    RobotKinect();

private:
    Kinect* kinect;
    Skeleton s;
};
