#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "Sign.h"

void BTV(cv::Mat& svec,cv::Size BTVkernel, float alpha,cv::Mat& dvec,cv::Size size)
{
	cv::Mat src;
	svec.reshape(3,size.height).convertTo(src,CV_32FC3);
	cv::Mat dst(size.height,size.width,CV_32FC3);

	const int kw = (BTVkernel.width-1)/2;
	const int kh = (BTVkernel.height-1)/2;

	float* weight = new float[BTVkernel.width*BTVkernel.height];
	for(int m=0,count=0;m<=kh;m++)
	{
		for(int l=kw;l+m>=0;l--)
		{
			weight[count]=pow(alpha,abs(m)+abs(l));     
			count++;
		}
	}

	for(int j=kh;j<src.rows-kh;j++)
	{
		float* s = src.ptr<float>(j);
		float* d = dst.ptr<float>(j);
		for(int i=kw;i<src.cols-kw;i++)
		{
			float r=0.0;
			float g=0.0;
			float b=0.0;

			const float sr=s[3*(i)+0];
			const float sg=s[3*(i)+1];
			const float sb=s[3*(i)+2];
			for(int m=0,count=0;m<=kh;m++)
			{
				float* s2 = src.ptr<float>(j-m);
				float* ss = src.ptr<float>(j+m);
				for(int l=kw;l+m>=0;l--)
				{
					r+=weight[count]*(Sign(sr,ss[3*(i+l)+0]) -Sign(s2[3*(i-l)+0],sr));
					g+=weight[count]*(Sign(sg,ss[3*(i+l)+1]) -Sign(s2[3*(i-l)+1],sg));
					b+=weight[count]*(Sign(sb,ss[3*(i+l)+2]) -Sign(s2[3*(i-l)+2],sb));
					count++;
				}
			}
			d[3*i+0]=r;
			d[3*i+1]=g;
			d[3*i+2]=b;
		}
	}
	dst.reshape(3,size.height*size.width).convertTo(dvec,CV_32FC3);
	delete[] weight;
}