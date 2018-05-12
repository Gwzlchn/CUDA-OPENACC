//��һ���ˣ����������һ�׵�
__device__ double3 gx_gy_gz_first_nucleus(const nucleus& first, const nucleus& second)
{
	double Q_squre = pow(Q_constant, 2);
	//����ƽ����
	double loc_squre_sum_first = pow(first.x, 2) + pow(first.y, 2) + pow(first.z, 2);
	
	//һ�׵�ƽ����
	double px_py_pz_squre_sum_first = pow(first.px, 2) + pow(first.py, 2) + pow(first.pz, 2);



	//��һ���� һ�׵��������㹫ʽ ��Ӧ g1 g3 g5
	double3 gx_gy_gz;
	gx_gy_gz.x = first.px * (1.0 - 1.0 / Q_squre * loc_squre_sum_first * px_py_pz_squre_sum_first
						* exp(A_hardness * (1.0 - pow(loc_squre_sum_first * px_py_pz_squre_sum_first/ Q_squre, 2))));

	gx_gy_gz.y = first.py * (1.0 - 1.0 / Q_squre * loc_squre_sum_first * px_py_pz_squre_sum_first
						* exp(A_hardness * (1.0 - pow(loc_squre_sum_first * px_py_pz_squre_sum_first / Q_squre, 2))));
	
	gx_gy_gz.z = first.pz * (1.0 - 1.0 / Q_squre * loc_squre_sum_first * px_py_pz_squre_sum_first
						* exp(A_hardness * (1.0 - pow(loc_squre_sum_first * px_py_pz_squre_sum_first / Q_squre, 2))));
	
	return gx_gy_gz;
}

//�ڶ����ˣ����������һ�׵�
__device__ double3 gx_gy_gz_second_nucleus(const nucleus& first, const nucleus& second)
{
	const double Q_squre = pow(Q_constant, 2);
	
	//����ƽ����
	const double loc_squre_sum_second = pow(second.x, 2) + pow(second.y, 2) + pow(second.z, 2);

	//һ�׵�ƽ����
	const double px_py_pz_squre_sum_second = pow(second.px, 2) + pow(second.py, 2) + pow(second.pz, 2);



	//�ڶ����� һ�׵��������㹫ʽ ��Ӧ g2 g4 g6
	double3 gx_gy_gz;
	gx_gy_gz.x = second.px * (1.0 - 1.0 / Q_squre * loc_squre_sum_second * px_py_pz_squre_sum_second
		* exp(A_hardness * (1.0 - pow(loc_squre_sum_second * px_py_pz_squre_sum_second / Q_squre, 2))));

	gx_gy_gz.y = second.py * (1.0 - 1.0 / Q_squre * loc_squre_sum_second * px_py_pz_squre_sum_second
		* exp(A_hardness * (1.0 - pow(loc_squre_sum_second * px_py_pz_squre_sum_second / Q_squre, 2))));

	gx_gy_gz.z = second.pz * (1.0 - 1.0 / Q_squre * loc_squre_sum_second * px_py_pz_squre_sum_second
		* exp(A_hardness * (1.0 - pow(loc_squre_sum_second * px_py_pz_squre_sum_second / Q_squre, 2))));

	return gx_gy_gz;
}

//��һ���ˣ���������Ķ��׵�
__device__ double3 fx_fy_fz_first_nucleus(const nucleus& first, const nucleus& second)
{
	const double Q_squre = pow(Q_constant, 2);

	//����ƽ����
	const double loc_squre_sum_first = pow(first.x, 2) + pow(first.y, 2) + pow(first.z, 2);
	const double loc_squre_sum_second = pow(second.x, 2) + pow(second.y, 2) + pow(second.z, 2);
	//һ�׵�ƽ����
	const double px_py_pz_squre_sum_first = pow(first.px, 2) + pow(first.py, 2) + pow(first.pz, 2);
	const double px_py_pz_squre_sum_second = pow(second.px, 2) + pow(second.py, 2) + pow(second.pz, 2);
	//���˾���ƽ��
	const double distance_squre = nucleus_distance(first, second);
	//���˾���ƽ����1.5 �η�,��Ӧ sqrt(((z1-z2)**2.d0+(y1-y2)**2.d0+(x1-x2)**2.d0)**3.d0)
	const double distance_1_5_power = pow(distance_squre, 1.5);

	//����ƽ���͵�1.5 �η� ����Ӧ sqrt((z1**2.d0+y1**2.d0+x1**2.d0)**3.d0)
	const double loc_1_5_power_first = pow(loc_squre_sum_first, 1.5);
	const double loc_1_5_power_second = pow(loc_squre_sum_second, 1.5);

	//��ʱ����1 ,��һ�����ӣ�һ�׵�ƽ���͵�ƽ�� / Q����
	const double temp1 = pow(px_py_pz_squre_sum_first, 2) / Q_squre;
	//��ʱ����2����һ�����ӣ�����ƽ���� * һ�׵�ƽ���� / Q������ƽ��
	const double temp2 = pow((loc_squre_sum_first * px_py_pz_squre_sum_first / Q_squre), 2);


	double3 fx_fy_fz;
	fx_fy_fz.x = first.x * ((Q_squre / 2.0 / A_hardness / pow(loc_squre_sum_first, 2) + temp1)
							* exp(A_hardness * (1.0 - temp2)) - 2.0 / loc_1_5_power_first)
				+ (first.x - second.x) / distance_1_5_power;

	fx_fy_fz.y = first.y * ((Q_squre / 2.0 / A_hardness / pow(loc_squre_sum_first, 2) + temp1)
							* exp(A_hardness * (1.0 - temp2)) - 2.0 / loc_1_5_power_first)
				+ (first.y - second.y) / distance_1_5_power;

	fx_fy_fz.z = first.z * ((Q_squre / 2.0 / A_hardness / pow(loc_squre_sum_first, 2) + temp1)
							* exp(A_hardness * (1.0 - temp2)) - 2.0 / loc_1_5_power_first)
				+ (first.z - second.z) / distance_1_5_power;

	return  fx_fy_fz;
}


//�ڶ����ˣ���������Ķ��׵�
__device__ double3 fx_fy_fz_second_nucleus(const nucleus& first, const nucleus& second)
{
	const double Q_squre = pow(Q_constant, 2);

	//����ƽ����
	const double loc_squre_sum_first = pow(first.x, 2) + pow(first.y, 2) + pow(first.z, 2);
	const double loc_squre_sum_second = pow(second.x, 2) + pow(second.y, 2) + pow(second.z, 2);
	//һ�׵�ƽ����
	const double px_py_pz_squre_sum_first = pow(first.px, 2) + pow(first.py, 2) + pow(first.pz, 2);
	const double px_py_pz_squre_sum_second = pow(second.px, 2) + pow(second.py, 2) + pow(second.pz, 2);
	//���˾���ƽ��
	const double distance_squre = nucleus_distance(first, second);
	//���˾���ƽ����1.5 �η�,��Ӧ sqrt(((z1-z2)**2.d0+(y1-y2)**2.d0+(x1-x2)**2.d0)**3.d0)
	const double distance_1_5_power = pow(distance_squre, 1.5);

	//����ƽ���͵�1.5 �η� ����Ӧ sqrt((z1**2.d0+y1**2.d0+x1**2.d0)**3.d0)
	const double loc_1_5_power_first = pow(loc_squre_sum_first, 1.5);
	const double loc_1_5_power_second = pow(loc_squre_sum_second, 1.5);

	//��ʱ����1 ,�ڶ������ӣ�һ�׵�ƽ���͵�ƽ�� / Q����
	//��Ӧ(pz1**2.d0+px1**2.d0+py1**2.d0)**2.d0/q**2.d0
	const double temp1 = pow(px_py_pz_squre_sum_second, 2) / Q_squre;
	//��ʱ����2���ڶ������ӣ�����ƽ���� * һ�׵�ƽ���� / Q������ƽ��
	const double temp2 = pow((loc_squre_sum_second * px_py_pz_squre_sum_second / Q_squre), 2);


	double3 fx_fy_fz;
	fx_fy_fz.x = second.x * ((Q_squre / 2.0 / A_hardness / pow(loc_squre_sum_second, 2) + temp1)
							* exp(A_hardness * (1.0 - temp2)) - 2.0 / loc_1_5_power_second)
				- (first.x - second.x) / distance_1_5_power;

	fx_fy_fz.y = second.y * ((Q_squre / 2.0 / A_hardness / pow(loc_squre_sum_second, 2) + temp1)
							* exp(A_hardness * (1.0 - temp2)) - 2.0 / loc_1_5_power_second)
				- (first.y - second.y) / distance_1_5_power;

	fx_fy_fz.z = second.z * ((Q_squre / 2.0 / A_hardness / pow(loc_squre_sum_second, 2) + temp1)
							* exp(A_hardness * (1.0 - temp2)) - 2.0 / loc_1_5_power_second)
				- (first.z - second.z) / distance_1_5_power;

	return  fx_fy_fz;

}



