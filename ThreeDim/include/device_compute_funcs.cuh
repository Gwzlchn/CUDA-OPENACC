﻿#ifndef DEVICE_COMPUTE_FUNCS_CUH
#define DEVICE_COMPUTE_FUNCS_CUH

#include "sci_const.h"
#include "nucleus.hpp"

#include <cmath>




//计算总动能
__device__ double E_kall(const nucleus& first, const nucleus& second);
//两核之间距离的平方
//返回（x1-x2)^2 +（y1-y2)^2 +（z1-z2)^2
__device__  double nucleus_distance(const nucleus& first, const nucleus& second);

//第一个核，三个坐标的二阶导
__device__ nucleus fx_first_nucleus(const nucleus& first, const nucleus& second);
//第二个核，三个坐标的二阶导
__device__ nucleus fx_second_nucleus(const nucleus& first, const nucleus& second);



__device__ void update_step_one(nucleus* step_one_fir, nucleus* step_one_sec);
__device__ void update_step_two(nucleus* step_two_fir, nucleus* step_two_sec);

#endif //DEVICE_COMPUTE_FUNCS_CUH


__device__ double E_kall(const nucleus& first, const nucleus& second)
{
	return E_total - (-1.0/sqrt(pow((first.z-nuclear_spacing/2.0*cos(PI*rotation)),2)+
								pow((first.x-nuclear_spacing-2.0*sin(PI*rotation)),2)+
								first.y*first.y+elec_elec*elec_elec))-
					(-1.0/sqrt(pow((second.z-nuclear_spacing/2.0*cos(PI*rotation)),2)+
								pow((second.x-nuclear_spacing/2.0*sin(PI*rotation)),2)+
								second.y*second.y+elec_elec*elec_elec))
					-(1.0/sqrt(nucleus_distance(first,second)+elec_nucl*elec_nucl))-
					(-1.0/sqrt(pow((first.z+nuclear_spacing/2.0*cos(PI*rotation)),2)+
								pow((first.x+nuclear_spacing/2.0*sin(PI*rotation)),2)+
								first.y*first.y+elec_elec*elec_elec))-
					(-1.0/sqrt(pow((second.z+nuclear_spacing/2.0*cos(PI*rotation)),2)+
								pow((second.x+nuclear_spacing/2.0*sin(PI*rotation)),2)+
								second.y*second.y + elec_elec*elec_elec));
}


__device__  double nucleus_distance(const nucleus& first, const nucleus& second)
{
	return (pow((first.x - second.x), 2) + pow((first.y - second.y), 2) + pow((first.z - second.z), 2));
}




__device__  nucleus fx_first_nucleus(const nucleus& first, const nucleus& second)
{
	nucleus fx_first;

	fx_first.x = (first.x - second.x)
			/ sqrt(pow((nucleus_distance(first, second) + elec_nucl*elec_nucl), 3)) 
		-(first.x - nuclear_spacing / 2.0 * sin(PI*rotation))
			/ sqrt(pow((pow((first.z - nuclear_spacing / 2.0 * cos(PI*rotation)), 2) + 
						pow((first.x - nuclear_spacing / 2.0 * sin(PI*rotation)), 2) + 
						first.y*first.y + elec_elec*elec_elec), 3)) 
		-(first.x + nuclear_spacing / 2.0 * sin(PI*rotation))
			/ sqrt(pow((pow((first.z + nuclear_spacing / 2.0 * cos(PI*rotation)), 2) + 
						pow((first.x + nuclear_spacing / 2.0 * sin(PI*rotation)), 2) + 
						first.y*first.y + elec_elec*elec_elec), 3));
	
	fx_first.y = (first.y - second.y)
			/ sqrt(pow((nucleus_distance(first, second) + elec_nucl*elec_nucl), 3)) 
		-first.y
			/ sqrt(pow((pow((first.z - nuclear_spacing / 2.0*cos(PI*rotation)), 2) + 
						pow((first.x - nuclear_spacing / 2.0*sin(PI*rotation)), 2) + 
						first.y*first.y + elec_elec*elec_elec), 3)) 
		-first.y
			/ sqrt(pow((pow((first.z + nuclear_spacing / 2.0*cos(PI*rotation)), 2) + 
						pow((first.x + nuclear_spacing / 2.0*sin(PI*rotation)), 3) + 
						first.y*first.y + elec_elec*elec_elec), 3));

	fx_first.z = (first.z-second.z)
			/ sqrt(pow((nucleus_distance(first, second) + elec_nucl*elec_nucl), 3))
		- (first.z - nuclear_spacing / 2.0 * cos(PI*rotation))
			/ sqrt(pow((pow((first.z - nuclear_spacing / 2.0 * cos(PI*rotation)), 2) +
						pow((first.x - nuclear_spacing / 2.0 * sin(PI*rotation)), 2) +
						first.y*first.y + elec_elec*elec_elec), 3))
		- (first.z + nuclear_spacing / 2.0 * cos(PI*rotation))
			/ sqrt(pow((pow((first.z + nuclear_spacing / 2.0 * cos(PI*rotation)), 2) +
						pow((first.x + nuclear_spacing / 2.0 * sin(PI*rotation)), 2) +
						first.y*first.y + elec_elec*elec_elec), 3));

	fx_first.px = fx_first.py = fx_first.pz = 0;
	return fx_first;
}
__device__  nucleus fx_second_nucleus(const nucleus& first, const nucleus& second)
{
	nucleus fx_second;
	fx_second.x = (second.x- first.x )
			/ sqrt(pow((nucleus_distance(first, second) + elec_nucl*elec_nucl), 3))
		- (second.x - nuclear_spacing / 2.0 * sin(PI*rotation))
			/ sqrt(pow((pow((second.z - nuclear_spacing / 2.0 * cos(PI*rotation)), 2) +
						pow((second.x - nuclear_spacing / 2.0 * sin(PI*rotation)), 2) +
						second.y*second.y + elec_elec*elec_elec), 3))
		- (second.x + nuclear_spacing / 2.0 * sin(PI*rotation))
			/ sqrt(pow((pow((second.z + nuclear_spacing / 2.0 * cos(PI*rotation)), 2) +
						pow((second.x + nuclear_spacing / 2.0 * sin(PI*rotation)), 2) +
						second.y*second.y + elec_elec*elec_elec), 3));

	fx_second.y = (second.y- first.y )
			/ sqrt(pow((nucleus_distance(first, second) + elec_nucl*elec_nucl), 3))
		- second.y
			/ sqrt(pow((pow((second.z - nuclear_spacing / 2.0*cos(PI*rotation)), 2) +
						pow((second.x - nuclear_spacing / 2.0*sin(PI*rotation)), 2) +
						second.y*second.y + elec_elec*elec_elec), 3))
		- second.y
			/ sqrt(pow((pow((second.z + nuclear_spacing / 2.0*cos(PI*rotation)), 2) +
						pow((second.x + nuclear_spacing / 2.0*sin(PI*rotation)), 3) +
						second.y*second.y + elec_elec*elec_elec), 3));

	fx_second.z = (first.z - second.z)
			/ sqrt(pow((nucleus_distance(first, second) + elec_nucl*elec_nucl), 3))
		- (second.z - nuclear_spacing / 2.0 * cos(PI*rotation))
			/ sqrt(pow((pow((second.z - nuclear_spacing / 2.0 * cos(PI*rotation)), 2) +
						pow((second.x - nuclear_spacing / 2.0 * sin(PI*rotation)), 2) +
						second.y*second.y + elec_elec*elec_elec), 3))
		- (second.z + nuclear_spacing / 2.0 * cos(PI*rotation))
			/ sqrt(pow((pow((second.z + nuclear_spacing / 2.0 * cos(PI*rotation)), 2) +
						pow((second.x + nuclear_spacing / 2.0 * sin(PI*rotation)), 2) +
						second.y*second.y + elec_elec*elec_elec), 3));
	fx_second.px = fx_second.py = fx_second.pz = 0;
	return fx_second;
}


void update_step_one(nucleus& step_one_fir, nucleus& step_one_sec)
{
	

}
//__device__ void update_step_two(nucleus* step_two_fir, nucleus* step_two_sec)


