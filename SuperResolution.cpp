#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"
#include "MultiplySparseMatrix.h"
#include "SubtractSign.h"
#include "SteepestDescentMethod.h"
#include "getPSNR.h"
#include"BTV.h"

void SuperResolution(cv::Mat src[],cv::Mat& dst,cv::SparseMat A[],int image_count,int iteration,float beta,int range,cv::Mat& ideal,cv::Size BTVkernel,float alpha,float lambda)
{
	resize(src[0],dst,dst.size());
	cv::Mat linear(dst.rows,dst.cols,CV_8UC3);
	resize(src[0],linear,linear.size());
	char lname[64];
	sprintf(lname,"%.2fdB",getPSNR(linear,ideal,range));
	cv::putText(linear,lname,cv::Point(10,20), cv::FONT_HERSHEY_DUPLEX,0.5,CV_RGB(255,255,255),2);
	imwrite("./output/SuperResolution/BilinearInterpolation.png",linear);

	cv::Mat dvec;
	dst.reshape(3,dst.cols*dst.rows).convertTo(dvec,CV_32FC3);

	cv::Mat* dvectmp=new cv::Mat[image_count];
	cv::Mat* svec = new cv::Mat[image_count]; //observed image
	cv::Mat* svec2 = new cv::Mat[image_count];

	for(int n=0;n<image_count;n++)
	{
		src[n].reshape(3,src[0].cols*src[0].rows).convertTo(svec[n],CV_32FC3);
		src[n].reshape(3,src[0].cols*src[0].rows).convertTo(svec2[n],CV_32FC3);
		dvectmp[n]=dvec.clone();
	}

	cv::Mat rvec(dst.rows*dst.cols,1,CV_32FC3);

	for(int i=0;i<iteration;i++)
	{
		std::cout<<"iteration"<<i<<std::endl;

		BTV(dvec,BTVkernel,alpha,rvec,dst.size());

		for(int n=0;n<image_count;n++)
		{
			//std::cout<<"n"<<n<<std::endl;
			MultiplySparseMatrix(A[n],dvec,svec2[n],false);

			cv::Mat diffsign(src[0].rows*src[0].cols,1,CV_32FC3);

			SubtractSign(svec2[n],svec[n],diffsign);

			MultiplySparseMatrix(A[n],diffsign,dvectmp[n],true);
		}

		SteepestDescentMethod(dvectmp,dvec,beta,image_count);

		dvec-=lambda*beta*rvec;

		dvec.reshape(3,dst.rows).convertTo(dst,CV_8UC3);
		
		if(!(i%10)){
			char name[64];
			sprintf(name,"%03d:%.2fdB",i,getPSNR(dst,ideal,range));
			cv::putText(dst,name,cv::Point(10,20), cv::FONT_HERSHEY_DUPLEX,0.5,CV_RGB(255,255,255),2);
			sprintf(name,"./output/SuperResolution/%03d.png",i);
			imwrite(name,dst);
		}
	}
}