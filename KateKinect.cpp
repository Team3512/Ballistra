#include "KateKinect.hpp"

KateKinect::KateKinect(){
	kinect = Kinect::GetInstance();
	s = new Skeleton();

}

std::pair <float,float> KateKinect::GetArmScale(){
    float RightScale;
    float LeftScale;
    //float Rightx;
    //float Righty;
    //float Leftx;
    //float Lefty;
    std::pair <float, float> p;

    RightScale = (s->GetHandRight().y - s->GetHipRight().y)/(s->GetShoulderRight().y - s->GetHipRight().y);
	 LeftScale = (s->GetHandLeft().y - s->GetHipLeft().y)/(s->GetShoulderLeft().y - s->GetHipLeft().y);
	 *s = kinect->GetSkeleton();

	 if (RightScale > 1){
		 RightScale  = 1;
	 }
	 if (LeftScale > 1){
		 LeftScale = 1;
	 }
	 if (RightScale < 0){
		 RightScale = 0;
	 }
	 if (LeftScale < 0){
		 LeftScale = 0;
	 }
	 if (!(LeftScale>=-1 && LeftScale<=1)){
		 LeftScale = 0;
	 }
	 if (!(RightScale>=-1 && RightScale<=1)){
		 RightScale = 0;
	 }
	 if (s->GetHandRight().x < s->GetShoulderRight().x){
		 RightScale = RightScale*-1;
	 }
	 if (s->GetHandLeft().x > s->GetShoulderLeft().x){
		 LeftScale = LeftScale*-1;
	 }
	 //Rightx = s->GetHandRight().x - s->GetShoulderRight().x;
	 //Righty = s->GetHandRight().y - s->GetShoulderRight().y;
	 //Leftx = s->GetHandLeft().x - s->GetShoulderLeft().x;
	 //Lefty = s->GetHandLeft().y - s->GetShoulderLeft().y;
	 //RightSlope = Righty/Rightx;
	 //LeftSlope = Lefty/Leftx;
	 p.first = RightScale;
	 p.second = LeftScale;
	 return p;
	 
	
}
