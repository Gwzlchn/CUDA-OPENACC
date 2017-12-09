#include <cuda_runtime.h>
#include <cstdio>
#include <cstdlib>
#include<iostream>
#include<fstream>
#include "GPUFunctions.h"
#include "common.h"
#include "HostFunctions.hpp"

//��ʱ�ñ���
double iStart;
double iElaps;
	
	
using namespace std;
int main()
{
   printf("Starting...\n");

    //ѡ���豸
    int dev = 0;
    cudaDeviceProp deviceProp;
    CHECK(cudaGetDeviceProperties(&deviceProp, dev));
    printf("Using Device %d: %s\n", dev, deviceProp.name);
    CHECK(cudaSetDevice(dev));

    // ��������nx��������
    int nx = 100000;
    int ny = 9;
    int nxy = nx * ny;
    int nBytes = nxy * sizeof(double);
    printf("Matrix size: nx %d ny %d\n", nx, ny);

	
	
	//���������ڴ�ռ�
	double *h_gpuRef;
	h_gpuRef = (double *)malloc(nBytes);
	//����GPU�ڴ�ռ�
	double *d_Result;
    CHECK(cudaMalloc((void **)&d_Result, nBytes));
	
    //�����������ʼ�����������˺������fx,px��ʼ��
    iStart = seconds();
    
	ifstream ReadInRandom;
	ReadInRandom.open("initOneCol.dat");
	int i=0;
	 while(!ReadInRandom.eof()) //��ȡ���ݵ�����  
        {  
  
            ReadInRandom>>h_gpuRef[i*ny];  
            //file>>tempChar[i];  
            i++;  
        }  
	
	
	ReadInRandom.close();
	
	StoreData(h_gpuRef,nx,ny,"test.dat");
	return 0;
	
}