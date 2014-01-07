#include "KateKinect.hpp"

KateKinect::KateKinect(){
	kinect = Kinect::GetInstance();
	s = new Skeleton();

}

std::pair <float,float> KateKinect::GetArmSlope(){
	 float RightSlope;
 	 float LeftSlope;
 	 float Rightx;
 	 float Righty;
 	 float Leftx;
 	 float Lefty;
 	 std::pair <float, float> p;
 	 
	 *s = kinect->GetSkeleton();
	 Rightx = s->GetHandRight().x - s->GetShoulderRight().x;
	 Righty = s->GetHandRight().y - s->GetShoulderRight().y;
	 Leftx = s->GetHandLeft().x - s->GetShoulderLeft().x;
	 Lefty = s->GetHandLeft().y - s->GetShoulderLeft().y;
	 RightSlope = Righty/Rightx;
	 LeftSlope = Lefty/Leftx;
	 p.first = RightSlope;
	 p.second = LeftSlope;
	 return p;
	 
	
}