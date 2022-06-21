#pragma once
 
//�ռ�λ��
enum relative_position
{
	inside_on,
	outside
};

//��������µĵ�
struct my_homogeneous_point
{
	float x;
	float y;
	float z;
	float ratio;

	enum relative_position pos = relative_position::inside_on;
};

//��������
class my_3Dvector
{
public:
	float dx;
	float dy;
	float dz;
	float len;
public:
	my_3Dvector() {}

	my_3Dvector(float x, float y, float z)
	{
		dx = x;
		dy = y;
		dz = z;

		len = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));
	}

	//start��ָ��end�������
	my_3Dvector(my_homogeneous_point start, my_homogeneous_point end)
	{
		dx = end.x - start.x;
		dy = end.y - start.y;
		dz = end.z - start.z;

		len = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));
	}

	//��� this X input_vector
	my_3Dvector cross_multiply(my_3Dvector input_vector)
	{
		float new_dx = dy * input_vector.dz - dz * input_vector.dy;
		float new_dy = dz * input_vector.dx - dx * input_vector.dz;
		float new_dz = dx * input_vector.dy - dy * input_vector.dx;
		return  my_3Dvector(new_dx, new_dy, new_dz);
	}

	//��� this * input_vector
	float dot_multiply(my_3Dvector input_vector)
	{
		return dx * input_vector.dx + dy * input_vector.dy + dz * input_vector.dz;
	}
};
