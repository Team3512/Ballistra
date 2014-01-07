#include <Kinect.h>

class KateKinect {
	public:
		std::pair<float, float> GetArmSlope();
		KateKinect();
	
	private:
		Kinect *kinect;
		Skeleton *s;
};