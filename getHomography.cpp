#include "C:\OpenCV2.3\include\opencv2\opencv.hpp"

cv::Mat getHomography(cv::Mat& src1,cv::Mat& src2)
{
	cv::Mat s1,s2;

	cv::cvtColor(src1,s1,CV_BGR2GRAY);
	cv::cvtColor(src2,s2,CV_BGR2GRAY);

	cv::SiftFeatureDetector detector;
	std::vector<cv::KeyPoint> keys1,keys2;
	detector.detect(s1,keys1);
	detector.detect(s2,keys2);

	cv::SiftDescriptorExtractor extractor;
	cv::Mat desp1,desp2;
	extractor.compute(s1,keys1,desp1);
	extractor.compute(s2,keys2,desp2);

	std::vector<cv::DMatch> matches;
	cv::FlannBasedMatcher matcher;
	matcher.match(desp1,desp2,matches);

	// check
	//cv::Mat tmp;
	//cv::drawMatches(s1,keys1,s2,keys2,matches,tmp);
	//imshow("match",tmp);
	//cv::waitKey();
	//imwrite("./output/match.jpg",tmp);

	std::vector<cv::Vec2f> points1(matches.size());
	std::vector<cv::Vec2f> points2(matches.size());

	for(size_t i=0;i<matches.size();i++)
	{
		points1[i][0]=keys1[matches[i].queryIdx].pt.x;
		points1[i][1]=keys1[matches[i].queryIdx].pt.y;
		
		points2[i][0]=keys2[matches[i].trainIdx].pt.x;
		points2[i][1]=keys2[matches[i].trainIdx].pt.y;
	}

	cv::Mat tmp=cv::findHomography(points1,points2,CV_RANSAC);

	cv::Mat Homography;
	tmp.convertTo(Homography,CV_32FC1);

	return Homography;

}