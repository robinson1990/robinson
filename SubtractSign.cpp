#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "Sign.h"
void SubtractSign(cv::Mat& src1,cv::Mat& src2,cv::Mat& dst)
{
	for(int j=0;j<src1.rows;j++)
	{
		float* s1=src1.ptr<float>(j);
		float* s2=src2.ptr<float>(j);
		float* d=dst.ptr<float>(j);
		for(int i=0;i<src1.cols;i++)
		{

			//std::cout<<"b4"<<std::endl;
			//std::cout<<d[3*i]<<std::endl;
			//std::cout<<d[3*i+1]<<std::endl;
			//std::cout<<d[3*i+2]<<std::endl;

			d[3*i]=Sign(s1[3*i],s2[3*i]);
			d[3*i+1]=Sign(s1[3*i+1],s2[3*i+1]);
			d[3*i+2]=Sign(s1[3*i+2],s2[3*i+2]);

			//std::cout<<"aft"<<std::endl;
			//std::cout<<d[3*i]<<std::endl;
			//std::cout<<d[3*i+1]<<std::endl;
			//std::cout<<d[3*i+2]<<std::endl;

		}
	}
}