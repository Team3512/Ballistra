#ifndef ROBOT_KINECT_HPP
#define ROBOT_KINECT_HPP

#include <utility>
#include <Kinect.h>

class RobotKinect {
public:
    std::pair<float, float> GetArmScale();
    RobotKinect();

private:
    Kinect *kinect;
    Skeleton *s;
};

#endif // ROBOT_KINECT_HPP
