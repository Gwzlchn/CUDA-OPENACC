
#ifndef SCI_CONST_H
#define SCI_CONST_H

#include <crt/host_defines.h>

//������й�
__device__ double stddev = 0.7;		//����

__device__ double mean = 2.0;			//��ֵ


//�������й�
__device__ double rotation = 0.0;	//ת��Ƕȣ���Ӧ֮ǰ��kk

__device__ double nuclear_spacing = 4.0;	//�˼��,��Ӧ֮ǰ��R

__device__ double PI = 3.1415926535897932384626433832795;	//Բ����

__device__ double elec_nucl =1.25;	//�������֮���������Ӧ֮ǰA

__device__ double pow_elec_nucl = 1.25 * 1.25;//A*A

__device__ double elec_elec = 0.1;	//���������֮���������Ӧ֮ǰA1

__device__ double pow_elec_elec = 0.1 * 0.1; //A1*A1

//!!ע�����4.0Ӧ����R,��windows��֧�֡�����������
__device__ double E_total = -1.0165 - 1.0 / 4.0;	//�������������� ��Ӧ֮ǰ E0



//��������õ�����
__device__ int  one_steps = 10000;

__device__ int	two_steps = 40000;

__device__ double DX = 0.0275438596;



#endif
