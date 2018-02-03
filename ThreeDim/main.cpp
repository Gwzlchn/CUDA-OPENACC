#include <cuda_runtime.h>
#include <cstdio>
#include <cstdlib>

#include "./include/common.hpp"
#include "./include/global_funcs.h"

int main()
{
	//��ʱ
	double Start, Elaps;

	printf("Starting...\n");

	//ѡ���豸
	int dev = 0;
	cudaDeviceProp deviceProp;
	CHECK(cudaGetDeviceProperties(&deviceProp, dev));
	printf("Using Device %d: %s\n", dev, deviceProp.name);
	CHECK(cudaSetDevice(dev));

	long pairs = 100000;
	
	compute_on_gpu_one(pairs,"10w");

	return 0;
}



