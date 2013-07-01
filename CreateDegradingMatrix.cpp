#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "getMove.h"

cv::SparseMat CreateDegradingMatrix(cv::Mat& src,int mfactor,cv::Mat& Homography)
{
	static int count=0;

	float div=1.0f/((float)(mfactor*mfactor));
	int size0=src.cols*src.rows;
    int size1=src.cols*src.rows/(mfactor*mfactor);
	int size[2]={size0,size1};

	cv::SparseMat A(2,size,CV_32FC1);

			cv::Point2f scoord;
			//scoord.x=(float)0.0f;
			//scoord.y=(float)0.0f;

			//cv::Point2f move=getMove(scoord,Homography,mfactor);

			//int x1=(int)(move.x+1);
			//int x0=(int)(move.x);
			//float a1=(float)(move.x-x0);
			//float a0=(float)(1.0-a1);
			//int y1=(int)(move.y+1);
			//int y0=(int)(move.y);
			//float b1=(float)(move.y-y0);
			//float b0=(float)(1.0-b1);

	const int step=src.cols/mfactor;

	for(int j=0;j<src.rows;j+=mfactor)
	{
		for(int i=0;i<src.cols;i+=mfactor)
		{
			cv::Point2f scoord;
			scoord.x=(float)i;
			scoord.y=(float)j;

			//std::cout<<"scoord"<<std::endl;
			//std::cout<<scoord.x<<std::endl;
			//std::cout<<scoord.y<<std::endl;

			//if(count){
			//	cv::waitKey();
			//	std::cout<<"(x,y)=("<<i<<","<<j<<")"<<std::endl;
			//}

			cv::Point2f move=getMove(scoord,Homography,mfactor);

			int x1;
			int x0;
			float a1;
			float a0;
			int y1;
			int y0;
			float b1;
			float b0;

			//x1
			if(move.x>=0)
			{
				x1=(int)(move.x+1);
			}
			else
			{
				x1=(int)(std::abs(move.x)+1);
			}

			//x0
			if(move.x>=0)
			{
				x0=(int)(move.x);
			}else
			{
				x0=(int)(std::abs(move.x));
			}

			//a1
			if(move.x>=0)
			{
				a1=(float)(move.x-x0);
			}
			else
			{
				a1=(float)(std::abs(move.x)-x0);
			}

			//a0
			if(move.x>=0)
			{
				a0=(float)(1.0-a1);
			}
			else
			{
				a0=(float)(1.0-a1);
			}

			//y1
			if(move.y>=0)
			{
				y1=(int)(move.y+1);
			}
			else
			{
				y1=(int)(std::abs(move.y)+1);
			}

			//y0
			if(move.y>=0)
			{
				y0=(int)(move.y);
			}
			else
			{
				y0=(int)(std::abs(move.y));
			}

			//b1
			if(move.y>=0)
			{
				b1=(float)(move.y-y0);
			}
			else
			{
				b1=(float)(std::abs(move.y)-y0);
			}

			//b0
			if(move.y>=0)
			{
				b0=(float)(1.0-b1);
			}
			else
			{
				b0=(float)(1.0-b1);
			}
			
			if(count)
			{
			std::cout<<"move(x,y)=("<<move.x<<","<<move.y<<")"<<std::endl;
			//std::cout<<std::abs(move.x)<<std::abs(move.y)<<std::endl;
			std::cout<<"x1="<<x1<<std::endl;
			std::cout<<"x0="<<x0<<std::endl;
			std::cout<<"a1="<<a1<<std::endl;
			std::cout<<"a0="<<a0<<std::endl;
			std::cout<<"y1="<<y1<<std::endl;
			std::cout<<"y0="<<y0<<std::endl;
			std::cout<<"b1="<<b1<<std::endl;
			std::cout<<"b0="<<b0<<std::endl;
			count--;
			}

			int y=src.cols*j+i;
			int s=step*j/mfactor+i/mfactor;

			//RGB
			//if(8<=i && i<=src.cols-8 && 4<=j && j<src.rows-4)
			//Thermal
			if(15<=i && i<=src.cols-15 && 11<=j && j<src.rows-11)

			{
				for(int l=0;l<mfactor;l++)
				{
					for(int k=0;k<mfactor;k++)
					{
						if(move.x>=0 && move.y>=0)
						{
						A.ref<float>(y-src.cols*y0+src.cols*l-x0+k,s)+=a0*b0*div;
						A.ref<float>(y-src.cols*y0+src.cols*l-x1+k,s)+=a1*b0*div;
						A.ref<float>(y-src.cols*y1+src.cols*l-x0+k,s)+=a0*b1*div;
						A.ref<float>(y-src.cols*y1+src.cols*l-x1+k,s)+=a1*b1*div;
						}
						else if(move.x>=0 && move.y<0)
						{
						A.ref<float>(y+src.cols*y0+src.cols*l-x0+k,s)+=a0*b0*div;
						A.ref<float>(y+src.cols*y0+src.cols*l-x1+k,s)+=a1*b0*div;
						A.ref<float>(y+src.cols*y1+src.cols*l-x0+k,s)+=a0*b1*div;
						A.ref<float>(y+src.cols*y1+src.cols*l-x1+k,s)+=a1*b1*div;
						}
						else if(move.x<0 && move.y>=0)
						{
						A.ref<float>(y-src.cols*y0+src.cols*l+x0+k,s)+=a0*b0*div;
						A.ref<float>(y-src.cols*y0+src.cols*l+x1+k,s)+=a1*b0*div;
						A.ref<float>(y-src.cols*y1+src.cols*l+x0+k,s)+=a0*b1*div;
						A.ref<float>(y-src.cols*y1+src.cols*l+x1+k,s)+=a1*b1*div;
						}
						else
						{
						A.ref<float>(y+src.cols*y0+src.cols*l+x0+k,s)+=a0*b0*div;
						A.ref<float>(y+src.cols*y0+src.cols*l+x1+k,s)+=a1*b0*div;
						A.ref<float>(y+src.cols*y1+src.cols*l+x0+k,s)+=a0*b1*div;
						A.ref<float>(y+src.cols*y1+src.cols*l+x1+k,s)+=a1*b1*div;
						}
					}
				}
			}
		}
	}
	count++;
	return A;
}