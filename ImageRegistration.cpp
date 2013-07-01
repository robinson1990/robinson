#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include"getPSNR.h"
#include"getHomography.h"

void ImageRegistration(cv::Mat& src1,cv::Mat& src2,int range)
{

	cv::Mat ideal=src2.clone();

	cv::Mat Homography(3,3,CV_32FC1);

	Homography=getHomography(src1,src2);

	cv::Mat dst;
	cv::warpPerspective(src1,dst,Homography,cv::Size(src1.size()));

	std::cout<<getPSNR(src1,ideal,range)<<std::endl;
	std::cout<<getPSNR(dst,ideal,range)<<std::endl;

	char name[64];
	sprintf(name,"%.3fdB",getPSNR(dst,ideal,range));
	cv::putText(dst,name,cv::Point(10,20), cv::FONT_HERSHEY_DUPLEX,0.7,CV_RGB(255,255,255),2);

	sprintf(name,"./output/ImageRegistration.png");
	imwrite(name,dst);
}