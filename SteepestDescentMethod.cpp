#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

void SteepestDescentMethod(cv::Mat src[],cv::Mat& dst,float beta,int image_count)
{

	for(int n=0;n<image_count;n++)
	{
		for(int j=0;j<dst.rows;j++)
		{

			float* s=src[n].ptr<float>(j);
			float* d=dst.ptr<float>(j);

			for(int i=0;i<dst.cols;i++)
			{
				d[3*i]-=beta*s[3*i];
				d[3*i+1]-=beta*s[3*i+1];
				d[3*i+2]-=beta*s[3*i+2];
			}
		}
	}
}