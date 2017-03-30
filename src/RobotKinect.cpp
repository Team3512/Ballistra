// Copyright (c) FRC Team 3512, Spartatroniks 2017. All Rights Reserved.

#include "RobotKinect.hpp"

RobotKinect::RobotKinect() { kinect = Kinect::GetInstance(); }

std::pair<float, float> RobotKinect::GetArmScale() {
    float RightScale;
    float LeftScale;
    // float Rightx;
    // float Righty;
    // float Leftx;
    // float Lefty;
    std::pair<float, float> p;

    RightScale = (s.GetHandRight().y - s.GetHipRight().y) /
                 (s.GetShoulderRight().y - s.GetHipRight().y);
    LeftScale = (s.GetHandLeft().y - s.GetHipLeft().y) /
                (s.GetShoulderLeft().y - s.GetHipLeft().y);
    s = kinect->GetSkeleton();

    if (LeftScale > 1) {
        LeftScale = 1;
    } else if (LeftScale < 0) {
        LeftScale = 0;
    }

    if (RightScale > 1) {
        RightScale = 1;
    } else if (RightScale < 0) {
        RightScale = 0;
    }

    if (s.GetHandRight().x < s.GetShoulderRight().x) {
        RightScale = -RightScale;
    }
    if (s.GetHandLeft().x > s.GetShoulderLeft().x) {
        LeftScale = -LeftScale;
    }
    // Rightx = s.GetHandRight().x - s.GetShoulderRight().x;
    // Righty = s.GetHandRight().y - s.GetShoulderRight().y;
    // Leftx = s.GetHandLeft().x - s.GetShoulderLeft().x;
    // Lefty = s.GetHandLeft().y - s.GetShoulderLeft().y;
    // RightSlope = Righty/Rightx;
    // LeftSlope = Lefty/Leftx;
    p.first = RightScale;
    p.second = LeftScale;

    return p;
}
