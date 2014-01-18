#include <Kinect.h>

class KateKinect {
	public:
		std::pair<float, float> GetArmScale();
		KateKinect();
	
	private:
		Kinect *kinect;
		Skeleton *s;
};
