#pragma once

//三维齐次坐标下的点
struct my_homogeneous_point
{
	float x{};
	float y{};
	float z = 0;
	float ratio{};
};

//定义向量
class my_3Dvector
{
public:
	float dx = 0;
	float dy = 0;
	float dz = 0;
	float len{};
public:
	my_3Dvector() {}
	my_3Dvector(float x, float y, float z)
	{
		dx = x;
		dy = y;
		dz = z;
		len = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));
	}

	//start点指向end点的向量
	my_3Dvector(my_homogeneous_point start, my_homogeneous_point end)
	{
		dx = end.x - start.x;
		dy = end.y - start.y;
		dz = end.z - start.z;
		len = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));
	}

	//叉乘 this X input_vector
	my_3Dvector cross_multiply(my_3Dvector input_vector)
	{
		float new_dx = dy * input_vector.dz - dz * input_vector.dy;
		float new_dy = dz * input_vector.dx - dx * input_vector.dz;
		float new_dz = dx * input_vector.dy - dy * input_vector.dx;
		return my_3Dvector(new_dx, new_dy, new_dz);
	}

	//点乘 this * input_vector
	float dot_multiply(my_3Dvector input_vector)
	{
		return dx * input_vector.dx + dy * input_vector.dy + dz * input_vector.dz;
	}
};