#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

float Sign(float src1,float src2)
{
	if(src1>src2)
	{
	return 1.0f;
	}else if(src1<src2)
	{
	return -1.0f;
	}else
	{
	return 0.0f;
	}
}