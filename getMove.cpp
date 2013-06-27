#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

cv::Point2f getMove(cv::Point2f src,cv::Mat& Homography,int mfactor)
{
	cv::Mat s(3,1,CV_32FC1);
	cv::Mat d(3,1,CV_32FC1);

	cv::Point2f move;
	cv::Point2f dst;

	s.at<float>(0,0)=(float)src.x;
	s.at<float>(1,0)=(float)src.y;
	s.at<float>(2,0)=(float)1.0f;

	d=Homography*s; 

	//std::cout<<Homography<<std::endl;

	dst.x=(d.at<float>(0,0)/d.at<float>(2,0)); 
	dst.y=(d.at<float>(1,0)/d.at<float>(2,0)); 

	//RGB
	//move.x=(dst.x-src.x)*160/1280;
	//move.y=(dst.y-src.y)*120/1024;

	//Thermal
	move.x=(dst.x-src.x)*2;
	move.y=(dst.y-src.y)*2;

	return move;
}