#include "Cluster_GPU.h"
#include <iostream>

Cluster_GPU::Cluster_GPU(DimensionConvertor* convertor):
a(0.0),
b(0.0),
c(0.0),
d(0.0),
size(0),
width(320*2),
height(240*2),
is_memory_clean(true){
	cvt = convertor;	
}

void Cluster_GPU::initMemory(){
	//std::cout << "begin init memory" << std::endl;
	is_memory_clean = false;
	host_inout = new float3[size];
	cudaMalloc(&device_input, size * sizeof(float3));
	cudaMalloc(&device_temp, size * sizeof(float3));
	cudaMalloc(&device_output, size * sizeof(float3));
	//std::cout << "exit init memory" << std::endl;
	
}

void Cluster_GPU::resetMemory(){	
	//std::cout << "begin reset memory" << std::endl;
	delete[] host_inout;
	cudaFree(device_input);
	cudaFree(device_temp);
	cudaFree(device_output);
	size = 0;
	is_memory_clean = true;
	//std::cout << "exit reset memory" << std::endl;
}

void Cluster_GPU::prepareConvertedPoints(Component* in, int type){
	//std::cout << "begin prepareConvertedPoints" << std::endl;
	if(!is_memory_clean)
		resetMemory();


	std::vector<PointRGB>* list;
	if(type == type::POINT3D)
		list = in->GetClusterVariables();
	else if(type == type::PIXEL)
		list = in->GetClusterPixels();

	size = list->size();
	float3 normal = in->GetNormal();
	a = normal.x;
	b = normal.y;
	c = normal.z;
	d = in->GetCompDistance();
	/*
	Get the number of points to convert first
	get a, b, c, d
	*/
	
	initMemory();		
		
	float3* ref;
	for(int i = 0; i < size; i++){
		//Move data from CPU to GPU memory
		ref = &list->at(i).coordinate;
		if(type == type::POINT3D){
			host_inout[i].x = ref->x / ref->z;
			host_inout[i].y = ref->y / ref->z;
			host_inout[i].z = ref->z;	
		} else if(type == type::PIXEL){
			host_inout[i].x = ref->x;
			host_inout[i].y = ref->y;
			


			host_inout[i].z = 1;
		}
	}

	if(type == type::PIXEL){
		cudaMemcpy(device_input, host_inout, size * sizeof(float3), cudaMemcpyHostToDevice);	
		cudaMemcpy(device_temp, device_input, size * sizeof(float3), cudaMemcpyDeviceToDevice);
	
		preparePsuedoPointCloud(mode::PSUEDO);	//depth = 1;
		
		setPsuedoDepth();						//depth set
		

		preparePsuedoPointCloud(mode::ACTUAL);	//conversion finished
	//	printDeviceMemory();

	} else if(type == type::POINT3D){
		cudaMemcpy(device_output, host_inout, size * sizeof(float3), cudaMemcpyHostToDevice);	
	}
	//std::cout << "exit prepareConvertedPoints" << std::endl;
}


void Cluster_GPU::setOutgoingPointRGB(Component* in){
	cudaMemcpy(host_inout, device_output, size * sizeof(float3), cudaMemcpyDeviceToHost);
	std::vector<PointRGB>* ref = in->GetClusterPixels();
	for(int i = 0; i < size; i++)
		ref->at(i).coordinate = host_inout[i];	
}


bool Cluster_GPU::convertPoints(Component* in){
	if(in->GetClusterID() == -1)
		return false;
	//std::cout << "begin convertPoints" << std::endl;
	prepareConvertedPoints(in, type::POINT3D);
	if(checkThreshold(in->GetD_threshold(), in)){	//Return the projected points if the threshold check passes
	//	std::cout << "convertPoints_THRESHOLDCHECK" << std::endl;
		prepareConvertedPoints(in, type::PIXEL);
		setOutgoingPointRGB(in);
		cudaDeviceSynchronize();
	//	std::cout << "exit convertPoints_true" << std::endl;
		return true;
	}
	cudaDeviceSynchronize();
	//std::cout << "exit convertPoints_false" << std::endl;
	return false;
}


void Cluster_GPU::preparePsuedoPointCloud(int mode){
	
	if(mode == mode::PSUEDO){
	//	std::cout << "begin preparePsuedoPointCloud_PSUEDO" << std::endl;
		cvt->projectiveToReal(device_input, device_input, width, height, size);
	} else if(mode == mode::ACTUAL) {
	//	std::cout << "begin preparePsuedoPointCloud_ACTUAL" << std::endl;
		cvt->projectiveToReal(device_output, device_output, width, height, size);

	}
	//std::cout << "exit preparePsuedoPointCloud" << std::endl;
}

void Cluster_GPU::printDeviceMemory(){
	float3* look = new float3[size];

	cudaMemcpy(look, device_output, size * sizeof(float3), cudaMemcpyDeviceToHost);


	for(int i = 0; i < size; i++)
		std::cout << look[i].x << ", " << look[i].y << " " << look[i].z << std::endl;


	delete[] look;
		


}







