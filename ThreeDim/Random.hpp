#ifndef RANDOM_H
#define RANDOM_H

#include <math.h>
#include <curand.h>

#include "nucleus.hpp"
#include "device_launch_parameters.h"
#include "sci_const.cuh"

//����˫����01���ȷֲ������
//����:	Array:˫��������	Size:���鳤��
void UniformRandomArrayD(double* Array, const long Size);

//����˫������̬�ֲ������
//����:	Array:˫��������	Size:���鳤��	Mean:��ֵ(0)	Stddev:����(0.7)
void NormalRandomArrayD(double* Array, const long Size, double Mean = 0, double Stddev = 0.7);

//����˫����˫��̬�ֲ������
//����:	Array1:˫��������1	Array2:˫��������2	Array3:˫��������3	Array2:˫��������4	
//Size:���鳤��	Nudis:�˼��(2)	Stddev:����(0.7)
__global__ void DoubleNormalRandomArrayD(double* Array1, double* Array2, double* Array3, double* Array4,
	const long Size, double Nudis = 2, double Stddev = 0.7);

//����˫�����ӵ��������
//����:	Array:��������	Size:���鳤�� Angle:ƫ�ƽ�(0)
void NucleiRandomD(nuclei* Array, const long Size, double Angle = 0);




//����˫����01���ȷֲ������
//����:	Array:˫��������	Size:���鳤��
void UniformRandomArrayD(double* Array, const long Size)
{
	curandGenerator_t gen;											//�������������
	curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_MRG32K3A);		//ָ���㷨
	curandSetPseudoRandomGeneratorSeed(gen, 11ULL);					//�������ʼ��
	curandGenerateUniformDouble(gen, Array, Size);					//����0-1���ȷֲ���������洢����������
	curandDestroyGenerator(gen);                         			//�ͷ�ָ��
	return;
}

//����˫������̬�ֲ������
//����:	Array:˫��������	Size:���鳤��	Mean:��ֵ(0)	Stddev:����(0.7)
void NormalRandomArrayD(double* Array, const long Size, double Mean, double Stddev)
{
	curandGenerator_t gen;											//�������������
	curandCreateGenerator(&gen, CURAND_RNG_PSEUDO_MRG32K3A);		//ָ���㷨
	curandSetPseudoRandomGeneratorSeed(gen, 11ULL);					//�������ʼ��
	curandGenerateNormalDouble(gen, Array, Size, Mean, Stddev);		//������̬�ֲ���������洢����������
	curandDestroyGenerator(gen);                         			//�ͷ�ָ��
	return;
}

//����˫����˫��̬�ֲ������
//����:	Array1:˫��������1	Array2:˫��������2	Array3:˫��������3	Array2:˫��������4	
//Size:���鳤��	Nudis:�˼��(2)	Stddev:����(0.7)
__global__ void DoubleNormalRandomArrayD(double* Array1, double* Array2, double* Array3, double* Array4,
	const long Size, double Nudis, double Stddev)
{
	int i = threadIdx.x;
	double temp1 = 1;
	double temp2 = 1;
	double temp3 = 1;

	Array1[i] = (Array1[i] - 0.5) * 20;
	Array3[i] = (Array3[i] - 0.5) * 20;

	temp1 = exp((-pow((Array1[i] - Nudis), Nudis)) / (Nudis * pow(Stddev, Nudis)))
		+ exp((-pow((Array1[i] + Nudis), Nudis)) / (Nudis * pow(Stddev, Nudis)));
	temp2 = exp((-pow((Array3[i] - Nudis), Nudis)) / (Nudis * pow(Stddev, Nudis)))
		+ exp((-pow((Array3[i] + Nudis), Nudis)) / (Nudis * pow(Stddev, Nudis)));

	if (Array2[i] > temp1 && Array4[i] > temp2)
	{
		Array1[i] = -99;
		Array3[i] = -99;
	}
	return;
}

//����˫�����ӵ��������
//����:	Array:��������	Size:���鳤�� Angle:ƫ�ƽ�
void NucleiRandomD(nuclei* Array, const long Size, double Angle)
{
	int i(0);
	int j(0);
	size_t DoubleSize = 2 * Size * sizeof(double);
	double *DTempArr1, *DTempArr2, *DTempArr3, *DTempArr4;
	cudaMalloc((void**)&DTempArr1, DoubleSize);
	cudaMalloc((void**)&DTempArr2, DoubleSize);
	cudaMalloc((void**)&DTempArr3, DoubleSize);
	cudaMalloc((void**)&DTempArr4, DoubleSize);

	while (i<Size)
	{
		UniformRandomArrayD(DTempArr1, 2 * Size);
		UniformRandomArrayD(DTempArr2, 2 * Size);
		UniformRandomArrayD(DTempArr3, 2 * Size);
		UniformRandomArrayD(DTempArr4, 2 * Size);

		int threadsPerBlock = 256;
		int threadsPerGrid = (2 * Size + threadsPerBlock - 1) / threadsPerBlock;
		DoubleNormalRandomArrayD << <threadsPerGrid, threadsPerBlock >> >(DTempArr1, DTempArr2, DTempArr3, DTempArr4, 2 * Size);
		while (i<Size && (i + j)<2 * Size)
		{
			if (DTempArr1[i + j] == -99)
			{
				j++;
			}
			else {
				Array[i].init_first.x = DTempArr1[i + j] * sin(Angle*PI);
				Array[i].init_first.y = 0;
				Array[i].init_first.z = DTempArr1[i + j] * cos(Angle*PI);
				Array[i].init_second.x = DTempArr3[i + j] * sin(Angle*PI);
				Array[i].init_second.y = 0;
				Array[i].init_second.z = DTempArr3[i + j] * cos(Angle*PI);
				i++;
			}
		}
	}
}
#endif //RANDOM_H
