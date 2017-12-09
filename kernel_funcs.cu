#include"kernel_funcs.h"
#include"device_funcs.cuh"
#include<curand.h>
#include "common.hpp"
#include"host_funcs.hpp"

__global__ void InitialKernel(double* Result,int nx,int ny)
{
	//��һ���Ѿ����������
    unsigned int ix = threadIdx.x + blockIdx.x * blockDim.x;
    if (ix < nx ){
        for (int iy = 0; iy <ny; iy++)
        {
            int idx = iy * nx + ix;
			//�ڶ���Ϊ��һ�и��Ե�px��ֵ��������ָ�����С����������ֱ�Ӹ�ֵ0�����㲿���жϼ�Щ��nan�ж��ܷ�������
            if((idx>=1*nx)&&(idx<2*nx)){
				if(Ekall(Result[idx-nx])>=0.0)
					Result[idx] = Px(double(Result[idx-nx]));
				else Result[idx] = 0.0;
			}
			//������Ϊ��һ�и��Ե�fx��ֵ������С�������ͬ��
			if((idx>=2*nx)&&(idx<3*nx)){
				if(Result[idx-1*nx]>0.0)
					Result[idx] = fx(double(Result[idx-2*nx]));
				else Result[idx] = 0.0;
			}
			
			//����������Ϊǰ���еĸ��ƣ�Ϊ��compute����׼��
			if((idx>=3*nx)&&(idx<4*nx)){
				if(Result[idx-2*nx]>0.0)
					Result[idx] = Result[idx-3*nx];
				else Result[idx] = 0.0;
			}
			if((idx>=4*nx)&&(idx<5*nx))
				Result[idx] = Result[idx-3*nx];
			
		}
	}

}



void NormalRandom(double *ip, const int size){
    

	curandGenerator_t gen;                                  //�������������
    curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_MRG32K3A);//����1��ָ���㷨
    curandSetPseudoRandomGeneratorSeed(gen, 11ULL);         //����2���������ʼ��
    curandGenerateNormalDouble(gen, ip, size, 0, 0.7);        //����3��������������洢���������У���1������Ϊ��ֵ���ڶ���Ϊ���
    curandDestroyGenerator(gen);                         	//�ͷ�ָ��
	return;
	
	
}


void  InitialMatrix(double* d_Result,int nx,int ny){
	NormalRandom(d_Result,nx);
	//����grid,block��С
	int dimx = 256;
    dim3 block(dimx, 1);
    dim3 grid((nx + block.x - 1) / block.x, 1);
	InitialKernel<<<grid,block>>>(d_Result,nx,ny);
	CHECK(cudaDeviceSynchronize());
	CHECK(cudaGetLastError());
	
	
	
	//�������ݽ���Ϊ�˲����ã�д��compute�����Ժ�϶����ñ�����������ˡ���
	int nxy = nx * ny;
    int nBytes = nxy * sizeof(double);
	double *h_gpuRef;
	h_gpuRef = (double *)malloc(nBytes);
	CHECK(cudaMemcpy(h_gpuRef, d_Result, nBytes, cudaMemcpyDeviceToHost));
	//��������
	double iStart = seconds();
	StoreData(h_gpuRef,nx,ny,"init.dat");
	double iElaps = seconds() - iStart;
    printf("STORE THE InitialKernel DATA elapsed %lf sec\n",iElaps);
	
	
}


__global__ void ComputeKernel(double* Result,int nx,int ny)
{
    unsigned int ix = threadIdx.x + blockIdx.x * blockDim.x;
	unsigned int idxOfXi  = 3 * nx + ix;
	unsigned int idxOfPxi = 4 * nx + ix;
	unsigned int idxOfXiTwo  = 5 * nx + ix;
	unsigned int idxOfPxiTwo = 6 * nx + ix;
	unsigned int idxOfTemp = 7 * nx + ix;


    if(ix<nx && Result[idxOfXi]!=0.0){
		for(int i=0;i<STEPSFIRST;i++){
			updateXi(Result[idxOfXi],Result[idxOfPxi]);
		}
		Result[idxOfXiTwo] = Result[idxOfXi];
		Result[idxOfPxiTwo] = Result[idxOfPxi];
		
		for(int i=0;i<STEPSSECOND;i++){
			updateXiAtStepTwo(Result[idxOfXiTwo],Result[idxOfPxiTwo],i*DX);
		}
		
		double TempE=0.5 * (pow(Result[idxOfPxiTwo],2.0)) - (1.0 / sqrt( pow(Result[idxOfXiTwo],2.0)+ pow(A,2.0)));
		if( TempE <= 0.0)
			Result[idxOfTemp]=-999;
	}
}



int CountZeros(double* h_Result,int nx)
{

	unsigned int idxOfXi  = nx ;
	unsigned int idxOfTemp = 7 * nx ;
	int count=0;
	for(int i=0;i<nx;i++){
		if(h_Result[idxOfXi+i] == 0.0f) count++;
		//if(h_Result[idxOfTemp+i] == -999) nonZeros++;
	}
	
	
	
	return count;
}

int CountTooBig(double* h_Result,int nx)
{

	unsigned int idxOfXi  = nx ;
	unsigned int idxOfTemp = 7 * nx ;
	int count=0;
	for(int i=0;i<nx;i++){
		//if(h_Result[idxOfXi+i] == 0.0) count++;
		if(h_Result[idxOfTemp+i] == -999) count++;
	}
	
	
	
	return count;
}









 void ComputeOnGPU1(double* Result,int nx,int ny,double* h_gpuRef){
	
	
	//����grid,block��С
	int dimx = 512;
    dim3 block(dimx);
    dim3 grid((nx + block.x - 1) / block.x, 1);
	
	double iStart = seconds();
	ComputeKernel<<<grid,block>>>(Result,nx,ny);
	 CHECK(cudaDeviceSynchronize());
	//����˺������󣬷�����Ϣ
    CHECK(cudaGetLastError());
	double iElaps = seconds() - iStart;
	printf("RungeOnGPU  elapsed %f sec\n",iElaps);
	
	// GPU���ݿ���������
	int nxy = nx * ny;
    int nBytes = nxy * sizeof(double);
	CHECK(cudaMemcpy(h_gpuRef, Result, nBytes, cudaMemcpyDeviceToHost));
	
	int zeros=0,nonzeros=0;
	zeros = CountZeros(h_gpuRef,nx);
	nonzeros = CountTooBig(h_gpuRef,nx);
	printf("The Number of Zeros is %d,\t The Number of NonZeros is %d \n",zeros,nonzeros);
	double per = (nx - zeros - nonzeros)/(nx - zeros);
	printf("Percentage is %lf  \n",per);
	
	//��������
	iStart = seconds();
	StoreData(h_gpuRef,nx,ny,"gpuStepTwo1202.dat");
	//StoreData(h_Random,1,ny,"h_Random.dat");
	iElaps = seconds() - iStart;
    printf("STORE THE ComputeKernel DATA elapsed %lf sec\n",iElaps);
	return;
}

