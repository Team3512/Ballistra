// Copyright (c) 2014-2017 FRC Team 3512. All Rights Reserved.

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
