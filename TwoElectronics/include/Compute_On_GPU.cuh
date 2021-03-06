﻿#ifndef __CALL_CPU_CUH
#define __CALL_GPU_CUH

#include "./Struct_Defines.h"
#include "device_launch_parameters.h"



__global__ void pairs_init(particle_pair* pair_array, const size_t size,
	const double min_r, const double min_p);

__global__   void pairs_first_step_every_step( particle_pair& init_one_pairs, particle_pair* every_step_arr, size_t size);


__global__ void pairs_first_step_on_gpu(particle_pair* first_step_pair_array, const size_t size);

__global__ void pre_second_step_qq(double* QQ_array);


__global__ void pre_second_step_E_arr_check
(const double* E1_array, const double* E2_array, double* E_check_array);


__global__ void pre_second_step_e1_arr(const double* QQ_array, const double EE0, double* E1_array);


__global__ void pre_second_step_e2_arr(const double* QQ_array, const double EE0, double* E2_array);

__global__ void pairs_second_step_on_gpu
(particle_pair* second_arr, const size_t size, double* E1_array, double* E2_array);




__global__ void pairs_second_step_on_gpu_every_step
(particle_pair* second_arr, const size_t size, double* E1_array, double* E2_array,
	particle_pair* every_step_arr);



__global__ void pairs_second_step_on_gpu_fliter
(const particle_pair* second_step_pair_array, particle_pair* second_step_pair_array_filter,
 const size_t size, unsigned long long* count_z, unsigned long long* count_zz);


#endif //CALL_CPU_CUH