#pragma once
#include <math.h>

using namespace std;

//三维空间中的点坐标
struct my_3D_point_coord
{
    float x;
    float y;
    float z;
};
 
//定义向量
class my_3Dvector
{
public:
	float dx{};
	float dy{};
	float dz{};
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
	my_3Dvector(my_3D_point_coord start, my_3D_point_coord end)
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
		return  my_3Dvector(new_dx, new_dy, new_dz);
	}

	//点乘 this * input_vector
	float dot_multiply(my_3Dvector input_vector)
	{
		return dx * input_vector.dx + dy * input_vector.dy + dz * input_vector.dz;
	}

	//获得向量本身的长度
	float get_length()
	{
		return sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));
	}

	//对向量归一化
	void normalized()
	{
		float dir_len = get_length();
		if (dir_len > 1e-4)
		{
			dx /= dir_len;
			dy /= dir_len;
			dz /= dir_len;
		}
	}
}; 
 
//阶乘函数
int Factorial(int m)
{
	int f = 1;
	for (int i = 1; i <= m; i++)
		f *= i;
	return f;
}

//Bernstein中的第一项参数
double Cnk(const int& n, const int& i)
{
	return double(Factorial(n) / (Factorial(i) * Factorial(n - i)));
}

//绘制Bezier曲线
void  DrawBezierCurve(vector<my_3D_point_coord> view_3D_points)
{
	double x, y, z;
	int rate = 800, n;
	n = view_3D_points.size() - 1;

	glBegin(GL_POINTS);
	for (double t = 0; t <= 1; t += 1.0 / rate)
	{
		x = y = z = 0;
		for(int i = 0; i <= n; i++)
		{
			double first_param = Cnk(n, i);
			double second_param = pow(t, i);
			double third_paramm = pow(1 - t, n - i);
			double prefix_val = first_param * second_param * third_paramm;
			x += view_3D_points[i].x * prefix_val;
			y += view_3D_points[i].y * prefix_val;
			z += view_3D_points[i].z * prefix_val;
		}
		glVertex3d(x, y, z);
	}
	glEnd();
}

//绘制Bezier曲面
void  DrawBezierSurface(my_3D_point_coord ctrl_points[4][4])
{
	double x, y, z;
	int rate = 400, m, n;
	m = n = 3;

	glBegin(GL_POINTS);
	for (double u = 0; u <= 1; u += 1.0 / rate)
		for (double v = 0; v <= 1; v += 1.0 / rate)
		{
			x = y = z = 0;
			for (int i = 0; i <= n; i++)
				for (int j = 0; j <= m; j++)
				{
					double prefix_val = Cnk(m, i) * pow(u, i) * pow(1 - u, m - i) * Cnk(n, j) * pow(v, j) * pow(1 - v, n - j);
					x += ctrl_points[i][j].x * prefix_val;
					y += ctrl_points[i][j].y * prefix_val;
					z += ctrl_points[i][j].z * prefix_val;
				}
			glVertex3d(x, y, z);
		}
	glEnd();
}

//绘制三次B样条曲线
void Draw_B3curves(vector<my_3D_point_coord> view_3D_points)
{
	int i, rate = 80;
	double x, y, z;
	double F03, F13, F23, F33;

	glBegin(GL_POINTS);
	x = (view_3D_points[0].x + 4.0 * view_3D_points[1].x + view_3D_points[2].x) / 6.0;
	y = (view_3D_points[0].y + 4.0 * view_3D_points[1].y + view_3D_points[2].y) / 6.0;
	z = (view_3D_points[0].z + 4.0 * view_3D_points[1].z + view_3D_points[2].z) / 6.0;

	for (i = 1; i < view_3D_points.size() - 2; i++)
	{
		for (double t = 0; t <= 1; t += 1.0 / rate)
		{
			F03 = (-t * t * t + 3 * t * t - 3 * t + 1) / 6;
			F13 = (3 * t * t * t - 6 * t * t + 4) / 6;
			F23 = (-3 * t * t * t + 3 * t * t + 3 * t + 1) / 6;
			F33 = t * t * t / 6;
			x = view_3D_points[i - 1].x * F03 + view_3D_points[i].x * F13 + view_3D_points[i + 1].x * F23 + view_3D_points[i + 2].x * F33;
			y = view_3D_points[i - 1].y * F03 + view_3D_points[i].y * F13 + view_3D_points[i + 1].y * F23 + view_3D_points[i + 2].y * F33;
			z = view_3D_points[i - 1].z * F03 + view_3D_points[i].z * F13 + view_3D_points[i + 1].z * F23 + view_3D_points[i + 2].z * F33;

			glVertex3d(x,y,z);
		}
	}	
	glEnd();
}

//绘制三次B样条曲面
void Draw_B3Surface(my_3D_point_coord ctrl_points[4][4])//绘制三次B样条曲线
{
	int i, j, rate = 80;
	double x, y, z;
	double t, v;
	double F03, F13, F23, F33;
	double V03, V13, V23, V33;

	glBegin(GL_POINTS);
	/*x = (view_3D_points[0].x + 4.0 * view_3D_points[1].x + view_3D_points[2].x) / 6.0;
	y = (view_3D_points[0].y + 4.0 * view_3D_points[1].y + view_3D_points[2].y) / 6.0;
	z = (view_3D_points[0].z + 4.0 * view_3D_points[1].z + view_3D_points[2].z) / 6.0;*/

	for (i = 1; i < 2; i++)
		for (j = 1; j < 2; j++)
			for (t = 0; t <= 1; t += 1.0 / rate)
			{
				F03 = (-t * t * t + 3 * t * t - 3 * t + 1) / 6;
				F13 = (3 * t * t * t - 6 * t * t + 4) / 6;
				F23 = (-3 * t * t * t + 3 * t * t + 3 * t + 1) / 6;
				F33 = t * t * t / 6;
				for (v = 0; v <= 1; v += 1.0 / rate)
				{
					V03 = (-v * v * v + 3 * v * v - 3 * v + 1) / 6;
					V13 = (3 * v * v * v - 6 * v * v + 4) / 6;
					V23 = (-3 * v * v * v + 3 * v * v + 3 * v + 1) / 6;
					V33 = v * v * v / 6;
					x = ctrl_points[i - 1][j - 1].x * F03 * V03 + ctrl_points[i][j].x * F13 * V13 + ctrl_points[i + 1][j + 1].x * F23 * V23 + ctrl_points[i + 2][j + 2].x * F33 * V33;
					y = ctrl_points[i - 1][j - 1].y * F03 * V03 + ctrl_points[i][j].y * F13 * V13 + ctrl_points[i + 1][j + 1].y * F23 * V23 + ctrl_points[i + 2][j + 2].y * F33 * V33;
					z = ctrl_points[i - 1][j - 1].z * F03 * V03 + ctrl_points[i][j].z * F13 * V13 + ctrl_points[i + 1][j + 1].z * F23 * V23 + ctrl_points[i + 2][j + 2].z * F33 * V33;

					glVertex3d(x, y, z);
				}
			}
	glEnd();
}

float B(int i, int k, double u, double du)
{
	if (k == 1)
	{
		//if (du * i < u && u < du * (i + 1))
		if (i <= u && u <= (i + 1))
			return 1.0;
		else
			return 0.0;
	}
	else
	{
		float sum1, sum2;
		//if (du * (i + k - 1) - du * i == 0)
		if (k == 1)
			//sum1 = 0.0;
			sum1 = B(i, k - 1, u, du) * (u - i);
		else
			//sum1 = B(i, k - 1, u, du) * (u - du * i) / (du * (i + k - 1) - du * i);
			sum1 = B(i, k - 1, u, du) * (u - i) / (k - 1);
		//if (du * (i + k) - du * (i - 1) == 0)
		if (k == 1)
			//sum2 = 0.0;
			sum2 = B(i + 1, k - 1, u, du) * (i + k - u);
		else
			//sum2 = B(i + 1, k - 1, u, du) * (du * (i + k) - u) / (du * (i + k) - du * (i - 1));
			sum2 = B(i + 1, k - 1, u, du) * (i + k - u) / (k - 1);
		return sum1 + sum2;
	}
}

void my_Draw_B3curves(vector<my_3D_point_coord> view_3D_points)//绘制三次B样条曲线
{
	double x, y, z;
	int i, n = view_3D_points.size() - 1, k = n + 1, rate = 800;
	double du = 1.0 / (n + k), u;

	//n = view_3D_points.size();
	//k = 4;
	k = 3;

	glBegin(GL_POINTS);
	//for (u = du * (k - 1); u <= du * (n + 1); u += 1.0 / rate)
	for (u = k - 1; u < n + 1; u += 1.0 / rate)
	{
		x = y = z = 0;
		for (i = 0; i <= n; i++)
		{
			float Biku = B(i, k, u, du);
			x += view_3D_points[i].x * Biku;
			y += view_3D_points[i].y * Biku;
			z += view_3D_points[i].z * Biku;
		}
		glVertex3d(x, y, z);
	}
	glEnd();
}

void my_Draw_B3Surface(my_3D_point_coord ctrl_points[4][4])//绘制三次B样条曲线
{
	double x, y, z;
	int i, j, rate = 200;
	int n = 3, k = n + 1;
	k = 3;
	double du = 1.0 / (n + k), u;
	double dv = 1.0 / (n + k), v;

	glBegin(GL_POINTS);
	//for (u = du * (k - 1); u <= du * (n + 1); u += 1.0 / rate)
		//for (v = dv * (k - 1); v <= dv * (n + 1); v += 1.0 / rate)
	for (u = k - 1; u <= n + 1; u += 1.0 / rate)
		for (v = k - 1; v <= n + 1; v += 1.0 / rate)
		{
			for (i = 0; i <= n; i++)
			{
				x = y = z = 0;
				float Biku = B(i, k, u, du);
				for (j = 0; j <= n; j++)
				{
					float Bjkv = B(j, k, v, dv);
					x += ctrl_points[i][j].x * Biku * Bjkv;
					y += ctrl_points[i][j].y * Biku * Bjkv;
					z += ctrl_points[i][j].z * Biku * Bjkv;
				}
			}
			glVertex3d(x, y, z);
		}
	glEnd();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////