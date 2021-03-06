﻿#ifndef GLOBAL_FUNCS_H
#define GLOBAL_FUNCS_H

#include "nucleus.hpp"

void compute_on_gpu_one(const long pairs,const char* file_name);

void NucleiRandomD(nuclei* Array, const long Size);

void NucleiFisrtStep(nuclei* first_array, const long size);

void NucleiSecondStep(nuclei* second_array, nuclei* second_array_fliter, const long size, double* aw, double* ds,unsigned long* count);

#endif //GLOBAL_FUNCS_H