#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <boost/thread/thread.hpp>
#include "math.h"
	
int main(){

	pcl::PointCloud<pcl::PointXYZRGB>::Ptr input (new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::io::loadPCDFile<pcl::PointXYZRGB>("input_cloud.pcd", *input);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr downsampled (new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::io::loadPCDFile<pcl::PointXYZRGB>("downsampled_cloud.pcd", *downsampled);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr interpolated (new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::io::loadPCDFile<pcl::PointXYZRGB>("interpolated_cloud.pcd", *interpolated);
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr upsampled (new pcl::PointCloud<pcl::PointXYZRGB>);
	pcl::io::loadPCDFile<pcl::PointXYZRGB>("upsampled_cloud.pcd", *upsampled);

	//double score = 0;
	//int count = 0;
	//std::cout << input->width <<std::endl;
	//for(int i=0; i < input->width; i++){
	//	if(input->points[i].z != 0){ 
	//	score += sqrt(pow((input->points[i].x - output->points[i].x), 2) +
	//						pow((input->points[i].y - output->points[i].y), 2) +
	//							pow((input->points[i].z - output->points[i].z), 2));
	//		count++;
	//		//std::cout << i << std::endl;
	//	}
	//}
	//std::cout << (double)(score / count) << std::endl;
	//
	//
	//boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer (new pcl::visualization::PCLVisualizer ("3D Viewer"));
	//viewer->initCameraParameters ();

	//int v1 = 0;
	//pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb_input(input);
	//viewer->createViewPort(0.0, 0.5, 0.5, 1.0, v1);
	////viewer->addText("image_cloud0", 10, 10, "v1 text", v1);
	//viewer->setBackgroundColor (0, 0, 0);
	//viewer->addPointCloud<pcl::PointXYZRGB> (input, rgb_input, "input", v1);
	//viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1.5, "input");
	//
	//
	//int v2 = 0;
	//pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb_downsampled(downsampled);
	//viewer->createViewPort(0.5, 0.5, 1.0, 1.0, v2);
	////viewer->addText("image_cloud1", 10, 10, "v2 text", v2);
	//viewer->setBackgroundColor (0, 0, 0);
	//viewer->addPointCloud<pcl::PointXYZRGB> (downsampled, rgb_downsampled, "downsampled", v2);
	//viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1.5, "downsampled");


	//
	//int v3 = 0;
	//pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb_output(output);
	//viewer->createViewPort(0.0, 0.0, 1.0, 0.5, v3);
	////viewer->addText("image_cloud1", 10, 10, "v2 text", v2);
	//viewer->setBackgroundColor (0, 0, 0);
	//viewer->addPointCloud<pcl::PointXYZRGB> (output, rgb_output, "output", v3);
	//viewer->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1.5, "output");

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer_input (new pcl::visualization::PCLVisualizer ("Input Viewer"));
	viewer_input->initCameraParameters ();
	pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb_input(input);
	viewer_input->addPointCloud<pcl::PointXYZRGB> (input, rgb_input, "input");
	viewer_input->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1.5, "input");

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer_downsampled (new pcl::visualization::PCLVisualizer ("Downsampled Viewer"));
	viewer_downsampled->initCameraParameters ();
	pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb_downsampled(downsampled);
	viewer_downsampled->addPointCloud<pcl::PointXYZRGB> (downsampled, rgb_downsampled, "downsampled");
	viewer_downsampled->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1.5, "downsampled");

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer_interpolated (new pcl::visualization::PCLVisualizer ("Interpolated Viewer"));
	viewer_interpolated->initCameraParameters ();
	pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb_interpolated(interpolated);
	viewer_interpolated->addPointCloud<pcl::PointXYZRGB> (interpolated, rgb_interpolated, "interpolated");
	viewer_interpolated->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1.5, "interpolated");

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer_upsampled (new pcl::visualization::PCLVisualizer ("Upsampled Viewer"));
	viewer_upsampled->initCameraParameters ();
	pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGB> rgb_upsampled(upsampled);
	viewer_upsampled->addPointCloud<pcl::PointXYZRGB> (upsampled, rgb_upsampled, "upsampled");
	viewer_upsampled->setPointCloudRenderingProperties (pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1.5, "upsampled");

	bool ShouldRun = true;
	
	while (ShouldRun) {  
		viewer_input->spinOnce (100);
		viewer_downsampled->spinOnce (100);
		viewer_interpolated->spinOnce(100);
		viewer_upsampled->spinOnce (100);

		boost::this_thread::sleep (boost::posix_time::microseconds (100000));
	}
	return 0;
}