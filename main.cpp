#include "main.h"
#include "CreateDegradingMatrixAndDegradedImage.h"
#include "SuperResolution.h"
#include "getHomography.h"
#include "CreateDegradingMatrix.h"

cv::RNG rng1;

int main(int argc,char** argv)
{

	const int image_count=10;
	cv::Mat Thermal_image[image_count];
	cv::Mat RGB_image[image_count];
	cv::Mat Homography[image_count];
	cv::SparseMat A[image_count];//degrading matrix
	int mfactor=2;//magnification factor

	std::string Thermal[]={"43","44","45","46","47","48","49","50","51","52"};
	std::string RGB[]={"T00043VB","T00044VB","T00045VB","T00046VB","T00047VB","T00048VB","T00049VB","T00050VB","T00051VB","T00052VB"};

	std::ostringstream Thermal_input[image_count];
	std::ostringstream RGB_input[image_count];

	for(int n=0;n<image_count;n++)
	{
		Thermal_input[n]<<"./input/"<<Thermal[n]<<".bmp";
	}

	for(int n=0;n<image_count;n++)
	{   
		Thermal_image[n]=cv::imread(Thermal_input[n].str());
	}

		for(int n=0;n<image_count;n++)
	{
		RGB_input[n]<<"./input/"<<RGB[n]<<".jpg";
	}

	for(int n=0;n<image_count;n++)
	{   
		RGB_image[n]=cv::imread(RGB_input[n].str());
	}

	cv::Mat ideal=cv::imread("./input/ideal.bmp");

	cv::Mat dst(ideal.rows,ideal.cols,CV_8UC3);

	int range=20;

	for(int n=0;n<image_count;n++)
	{   
		std::cout<<"image_count="<<n<<std::endl;

		//Homography[n]=getHomography(degimage[n],degimage[image_count-1]);

		//Homography[n]=getHomography(RGB_image[0],RGB_image[n]);

		Homography[n]=getHomography(Thermal_image[0],Thermal_image[n]);

		std::cout<<Homography[n]<<std::endl;

		if(!n)
		{
		Homography[n]=(cv::Mat_<float>(3,3)<<1.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,1.0f);
		}

		A[n]=CreateDegradingMatrix(ideal,mfactor,Homography[n]);
	}

	int iteration=100;
	float beta=1.3f;//convergence speed

	//BTV
	cv::Size BTVkernel(7,7);
	float alpha=0.7f;
	float lambda=0.03f;

	SuperResolution(Thermal_image,dst,A,image_count,iteration,beta,range,ideal,BTVkernel,alpha,lambda);

}


