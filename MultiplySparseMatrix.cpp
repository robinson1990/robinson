#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

void MultiplySparseMatrix(cv::SparseMat& A,cv::Mat& svec,cv::Mat& dvec,bool isTranspose)
{
	dvec.setTo(0);
	cv::SparseMatIterator it=A.begin(),it_end=A.end();

	static int count=0;

	if(!isTranspose)
	{
		count++;

		for(;it!=it_end;++it)
		{
			int i=it.node()->idx[0];//svec index
			int j=it.node()->idx[1];//dvec index

			//std::cout<<i<<std::endl;
			//std::cout<<j<<std::endl;

			float* d=dvec.ptr<float>(j);//save the address of j   
			float* s=svec.ptr<float>(i);//save the address of i   

			for(int i=0;i<3;i++)
			{
				d[i]+=it.value<float>()*s[i];
			}
		}
	}
	else
	{
		int count=0;

		for(;it!=it_end;++it)
		{
			count++;
			int i=it.node()->idx[1];//svec index
			int j=it.node()->idx[0];//dvec index

			float* d=dvec.ptr<float>(j);//save the address of j   
			float* s=svec.ptr<float>(i);//save the address of i   

			for(int i=0;i<3;i++)
			{
				d[i]+=it.value<float>()*s[i];
			}
		}
	}
}