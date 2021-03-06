﻿#ifndef __INIT_FIRST_SECOND_CUH
#define __INIT_FIRST_SECOND_CUH

#include "device_launch_parameters.h"
#include "Struct_Defines.h"




//最小值 rr pp

void get_min_r_min_p(int nx, int ny, double& min_r, double& min_p);


//获得六个随机数，如果有1 则置0 cuda 范围是（0，1] 但fortran随机数范围是[0, 1)
__device__ void get_six_random(double2& two_random, double4& four_random, const int& seed);

// x y z Px Py Pz分配 seed 种子，min_r 对应 rr min_p 对应 pp
__device__ void distribution(particle& first, particle& second,
	 int seed,  double min_r,  double min_p);





//第一个核，三个坐标的一阶导
__device__ double3 gx_gy_gz_first_nucleus(const particle& first, const particle& second, const double& t);

//第二个核，三个坐标的一阶导
__device__ double3 gx_gy_gz_second_nucleus(const particle& first, const particle& second, const double& t);

//第一个核，三个坐标的二阶导
__device__ double3 fx_fy_fz_first_nucleus(const particle& first, const particle& second, const double& t);

//第二个核，三个坐标的二阶导
__device__ double3 fx_fy_fz_second_nucleus(const particle& first, const particle& second, const double& t);








#endif //__INIT_FIRST_SECOND_CUH