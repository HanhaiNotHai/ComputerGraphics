#define GLUT_DISABLE_ATEXIT_HACK
#include <List>
#include <vector>
#include "GLUT.H"
#include <math.h>
#include <string.h>
#include "My_3DVector.h"

#define pi 3.1415926535
constexpr auto bianchang = 101;

using namespace std;

int nearplane_width = 600; //视景体宽度
int nearplane_height = 600; //视景体高度
int nearplane_distance = 500; //视景体近平面与视点距离
int farplane_distance = nearplane_distance + 300; //视景体远平面与视点距离

float eye_x = 20;
float eye_y = 20;
float eye_z = 20;
float theta = 0.1; //视点偏转角度

//定义面的结构体，包含该面的各个顶点和法向量
struct my_face_homogeneous
{
	list<my_homogeneous_point> mList;//各个顶点按照逆时针的顺序储存
	my_3Dvector n; //定义面法向量
};

my_homogeneous_point facepoint, a, b, c;//用来存储临时点
my_3Dvector N1, N2, N3;//用来存储向量以及计算面的法向量
my_face_homogeneous tempt; //用于存储面的信息
vector<my_face_homogeneous> model, model2; //用来存储三维图形的10个面
my_3Dvector v(eye_x, eye_y, eye_x);//用向量v存储视点方向

struct coordinate_system
{
	my_homogeneous_point o;//原点
	my_3Dvector x, y, z;//坐标轴
	void init()
	{
		x.dx = 1;
		x.dy = 0;
		x.dz = 0;

		y.dx = 0;
		y.dy = 1;
		y.dz = 0;

		z.dx = 0;
		z.dy = 0;
		z.dz = 1;
	}
};

struct zhenhuancun
{
	float x{}, y{}, z{};
	float zb{};
	float r{}, g{}, b{};
};

coordinate_system jubu;
zhenhuancun zhc[bianchang][bianchang];

//初始化三维图形顶点坐标
void init(void)
{
	//面一
	facepoint.x = 45;
	facepoint.y = 0;
	facepoint.z = 45;
	tempt.mList.push_back(facepoint);
	a = facepoint;

	facepoint.x = 45;
	facepoint.y = 50;
	facepoint.z = 45;
	tempt.mList.push_back(facepoint);
	b = facepoint;

	facepoint.x = 45;
	facepoint.y = 50;
	facepoint.z = 90;
	tempt.mList.push_back(facepoint);
	c = facepoint;

	facepoint.x = 45;
	facepoint.y = 0;
	facepoint.z = 90;
	tempt.mList.push_back(facepoint);

	N1 = my_3Dvector(a, b);//N1用来存储当前平面上的一个向量
	N2 = my_3Dvector(a, c);//N2用来存储当前平面上的一个向量
	N3 = N1.cross_multiply(N2);//N3为当前平面的法向量
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//面二
	facepoint.x = 80;
	facepoint.y = 0;
	facepoint.z = 20;
	tempt.mList.push_back(facepoint);
	a = facepoint;

	facepoint.x = 80;
	facepoint.y = 50;
	facepoint.z = 20;
	tempt.mList.push_back(facepoint);
	b = facepoint;

	facepoint.x = 60;
	facepoint.y = 50;
	facepoint.z = 20;
	tempt.mList.push_back(facepoint);
	c = facepoint;

	facepoint.x = 60;
	facepoint.y = 0;
	facepoint.z = 20;
	tempt.mList.push_back(facepoint);

	N1 = my_3Dvector(a, b);//N1用来存储当前平面上的一个向量
	N2 = my_3Dvector(a, c);//N2用来存储当前平面上的一个向量
	N3 = N1.cross_multiply(N2);//N3为当前平面的法向量
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//面三
	facepoint.x = 60;
	facepoint.y = 0;
	facepoint.z = 20;
	tempt.mList.push_back(facepoint);
	a = facepoint;

	facepoint.x = 60;
	facepoint.y = 50;
	facepoint.z = 20;
	tempt.mList.push_back(facepoint);
	b = facepoint;

	facepoint.x = 60;
	facepoint.y = 50;
	facepoint.z = 45;
	tempt.mList.push_back(facepoint);
	c = facepoint;

	facepoint.x = 60;
	facepoint.y = 0;
	facepoint.z = 45;
	tempt.mList.push_back(facepoint);
	N1 = my_3Dvector(a, b);//N1用来存储当前平面上的一个向量
	N2 = my_3Dvector(a, c);//N2用来存储当前平面上的一个向量
	N3 = N1.cross_multiply(N2);//N3为当前平面的法向量
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//面四
	facepoint.x = 60;
	facepoint.y = 0;
	facepoint.z = 45;
	tempt.mList.push_back(facepoint);
	a = facepoint;

	facepoint.x = 60;
	facepoint.y = 50;
	facepoint.z = 45;
	tempt.mList.push_back(facepoint);
	b = facepoint;

	facepoint.x = 45;
	facepoint.y = 50;
	facepoint.z = 45;
	tempt.mList.push_back(facepoint);
	c = facepoint;

	facepoint.x = 45;
	facepoint.y = 0;
	facepoint.z = 45;
	tempt.mList.push_back(facepoint);

	N1 = my_3Dvector(a, b);//N1用来存储当前平面上的一个向量
	N2 = my_3Dvector(a, c);//N2用来存储当前平面上的一个向量
	N3 = N1.cross_multiply(N2);//N3为当前平面的法向量
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//面五
	facepoint.x = 80;
	facepoint.y = 0;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);
	a = facepoint;

	facepoint.x = 80;
	facepoint.y = 50;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);
	b = facepoint;

	facepoint.x = 80;
	facepoint.y = 50;
	facepoint.z = 20;
	tempt.mList.push_back(facepoint);
	c = facepoint;

	facepoint.x = 80;
	facepoint.y = 0;
	facepoint.z = 20;
	tempt.mList.push_back(facepoint);

	N1 = my_3Dvector(a, b);//N1用来存储当前平面上的一个向量
	N2 = my_3Dvector(a, c);//N2用来存储当前平面上的一个向量
	N3 = N1.cross_multiply(N2);//N3为当前平面的法向量
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//面六
	facepoint.x = 45;
	facepoint.y = 0;
	facepoint.z = 90;
	tempt.mList.push_back(facepoint);
	a = facepoint;

	facepoint.x = 45;
	facepoint.y = 50;
	facepoint.z = 90;
	tempt.mList.push_back(facepoint);
	b = facepoint;

	facepoint.x = 0;
	facepoint.y = 50;
	facepoint.z = 90;
	tempt.mList.push_back(facepoint);
	c = facepoint;

	facepoint.x = 0;
	facepoint.y = 0;
	facepoint.z = 90;
	tempt.mList.push_back(facepoint);

	N1 = my_3Dvector(a, b);//N1用来存储当前平面上的一个向量
	N2 = my_3Dvector(a, c);//N2用来存储当前平面上的一个向量
	N3 = N1.cross_multiply(N2);//N3为当前平面的法向量
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//面七
	facepoint.x = 0;
	facepoint.y = 0;
	facepoint.z = 90;
	tempt.mList.push_back(facepoint);
	a = facepoint;

	facepoint.x = 0;
	facepoint.y = 50;
	facepoint.z = 90;
	tempt.mList.push_back(facepoint);
	b = facepoint;

	facepoint.x = 0;
	facepoint.y = 50;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);
	c = facepoint;

	facepoint.x = 0;
	facepoint.y = 0;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);

	N1 = my_3Dvector(a, b);//N1用来存储当前平面上的一个向量
	N2 = my_3Dvector(a, c);//N2用来存储当前平面上的一个向量
	N3 = N1.cross_multiply(N2);//N3为当前平面的法向量
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//面八
	facepoint.x = 0;
	facepoint.y = 0;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);
	a = facepoint;//a用来存储当前平面上的一个点

	facepoint.x = 0;
	facepoint.y = 50;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);
	b = facepoint;//b用来存储当前平面上的一个点

	facepoint.x = 80;
	facepoint.y = 50;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);
	c = facepoint;//c用来存储当前平面上的一个点

	facepoint.x = 80;
	facepoint.y = 0;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);

	N1 = my_3Dvector(a, b);//N1用来存储当前平面上的一个向量
	N2 = my_3Dvector(a, c);//N2用来存储当前平面上的一个向量
	N3 = N1.cross_multiply(N2);//N3为当前平面的法向量
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//面九
	facepoint.x = 0;
	facepoint.y = 50;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);
	a = facepoint;

	facepoint.x = 0;
	facepoint.y = 50;
	facepoint.z = 90;
	tempt.mList.push_back(facepoint);
	b = facepoint;

	facepoint.x = 45;
	facepoint.y = 50;
	facepoint.z = 90;
	tempt.mList.push_back(facepoint);
	c = facepoint;

	facepoint.x = 45;
	facepoint.y = 50;
	facepoint.z = 45;
	tempt.mList.push_back(facepoint);

	facepoint.x = 60;
	facepoint.y = 50;
	facepoint.z = 45;
	tempt.mList.push_back(facepoint);

	facepoint.x = 60;
	facepoint.y = 50;
	facepoint.z = 20;
	tempt.mList.push_back(facepoint);

	facepoint.x = 80;
	facepoint.y = 50;
	facepoint.z = 20;
	tempt.mList.push_back(facepoint);

	facepoint.x = 80;
	facepoint.y = 50;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);

	N1 = my_3Dvector(a, b);//N1用来存储当前平面上的一个向量
	N2 = my_3Dvector(a, c);//N2用来存储当前平面上的一个向量
	N3 = N1.cross_multiply(N2);//N3为当前平面的法向量
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//面十
	facepoint.x = 0;
	facepoint.y = 0;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);
	a = facepoint;

	facepoint.x = 80;
	facepoint.y = 0;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);
	b = facepoint;

	facepoint.x = 80;
	facepoint.y = 0;
	facepoint.z = 20;
	tempt.mList.push_back(facepoint);
	c = facepoint;

	facepoint.x = 60;
	facepoint.y = 0;
	facepoint.z = 20;
	tempt.mList.push_back(facepoint);

	facepoint.x = 60;
	facepoint.y = 0;
	facepoint.z = 45;
	tempt.mList.push_back(facepoint);

	facepoint.x = 45;
	facepoint.y = 0;
	facepoint.z = 45;
	tempt.mList.push_back(facepoint);

	facepoint.x = 45;
	facepoint.y = 0;
	facepoint.z = 90;
	tempt.mList.push_back(facepoint);

	facepoint.x = 0;
	facepoint.y = 0;
	facepoint.z = 90;
	tempt.mList.push_back(facepoint);

	N1 = my_3Dvector(a, b);//N1用来存储当前平面上的一个向量
	N2 = my_3Dvector(a, c);//N2用来存储当前平面上的一个向量
	N3 = N1.cross_multiply(N2);//N3为当前平面的法向量
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();
}

//绘制坐标系
void draw_coordinate()
{
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0); //红色x轴
	glVertex3f(nearplane_width, 0.0, 0.0);
	glVertex3f(-nearplane_width, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);//绿色y轴
	glVertex3f(0.0, nearplane_height, 0.0);
	glVertex3f(0.0, -nearplane_height, 0.0);

	glColor3f(0.0, 0.0, 1.0);//蓝色z轴
	glVertex3f(0.0, 0.0, nearplane_height);
	glVertex3f(0.0, 0.0, -nearplane_height);

	glEnd();
}

void xiaoyin()
{
	for (int i = 0; i < 10; i++)
	{
		//float num = model[i].n.dot_multiply(v);//模型第i个面法向量与视向量点乘
		if (model[i].n.dot_multiply(v) > 0)//绘制可见面
		{
			//以下代码是为了让不同平面呈现不同颜色
			switch (i)
			{
			case 0:
			{
				glColor3f(0, 0, 0);
				break;
			}
			case 1:
			{
				glColor3f(1, 0, 0);
				break;
			}
			case 2:
			{
				glColor3f(0, 1, 0);
				break;
			}
			case 3:
			{
				glColor3f(0, 0, 1);
				break;
			}
			case 4:
			{
				glColor3f(1, 1, 0);
				break;
			}
			case 5:
			{
				glColor3f(1, 0, 1);
				break;
			}
			case 6:
			{
				glColor3f(0, 1, 1);
				break;
			}
			case 7:
			{
				glColor3f(0.5, 0, 0);
				break;
			}
			case 8:
			{
				glColor3f(0, 0.5, 0);
				break;
			}
			case 9:
			{
				glColor3f(0, 0, 0.5);
				break;
			}
			}

			glBegin(GL_POLYGON);
			list<my_homogeneous_point>::iterator iter = model[i].mList.begin();
			for (; iter != model[i].mList.end(); iter++)
				glVertex3f((*iter).x, (*iter).y, (*iter).z);
			glEnd();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
struct my_3Dvector matrix_multiply_vector(float matrix[][3], struct my_3Dvector input_v)
{
	struct my_3Dvector translated_v;
	translated_v.dx = matrix[0][0] * input_v.dx + matrix[0][1] * input_v.dy + matrix[0][2] * input_v.dz;
	translated_v.dy = matrix[1][0] * input_v.dx + matrix[1][1] * input_v.dy + matrix[1][2] * input_v.dz;
	translated_v.dz = matrix[2][0] * input_v.dx + matrix[2][1] * input_v.dy + matrix[2][2] * input_v.dz;
	return translated_v;
}

//齐次旋转
void my_rotate_homogeneous(struct my_3Dvector& polygon, char axis, float angle)
{
	angle = angle * pi / 180;

	//装配生成变换矩阵
	float translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(int) * 9);

	switch (axis)
	{
	case'x':
	{
		translate_matrix[0][0] = 1;
		translate_matrix[1][1] = cos(angle);
		translate_matrix[2][2] = cos(angle);
		translate_matrix[1][2] = -sin(angle);
		translate_matrix[2][1] = sin(angle);
		break;
	}
	case'y':
	{
		translate_matrix[1][1] = 1;
		translate_matrix[2][2] = cos(angle);
		translate_matrix[0][0] = cos(angle);
		translate_matrix[2][0] = -sin(angle);
		translate_matrix[0][2] = sin(angle);
		break;
	}
	case'z':
	{
		translate_matrix[2][2] = 1;
		translate_matrix[0][0] = cos(angle);
		translate_matrix[1][1] = cos(angle);
		translate_matrix[0][1] = -sin(angle);
		translate_matrix[1][0] = sin(angle);
		break;
	}
	}

	//遍历并变换多边形每个顶点
	struct my_3Dvector input_v;

	input_v.dx = polygon.dx;
	input_v.dy = polygon.dy;
	input_v.dz = polygon.dz;

	input_v = matrix_multiply_vector(translate_matrix, polygon);

	polygon.dx = input_v.dx;
	polygon.dy = input_v.dy;
	polygon.dz = input_v.dz;
}

void my_axis_rotate_homogeneous(struct my_3Dvector& polygon, double a, double b, double c)
{
	float angle1 = atan2(b, c) * 180 / pi;
	float angle2 = atan2(a, sqrt(b * b + c * c)) * 180 / pi;

	//a,b,c转到z轴
	//my_rotate_homogeneous(polygon, 'x', angle1);
	//my_rotate_homogeneous(polygon, 'y', -angle2);

	//z轴转到a,b,c
	my_rotate_homogeneous(polygon, 'y', angle2);
	my_rotate_homogeneous(polygon, 'x', -angle1);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////
void my_rotate_y_homogeneous(float angle)
{
	//装配生成旋转矩阵 
	float translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(int) * 9);
	float PAI = 3.14159;
	angle = angle * PAI / 180;
	translate_matrix[1][1] = 1;
	translate_matrix[0][0] = cos(angle);
	translate_matrix[0][2] = sin(angle);
	translate_matrix[2][0] = -sin(angle);
	translate_matrix[2][2] = cos(angle);

	//遍历多边形每个顶点
	for (int i = 0; i < 10; i++)
	{
		list<my_homogeneous_point>::iterator iter = model[i].mList.begin();
		for (; iter != model[i].mList.end(); iter++)
		{
			struct my_3Dvector input_v;
			input_v.dx = (*iter).x;
			input_v.dy = (*iter).y;
			input_v.dz = (*iter).z;
			input_v = matrix_multiply_vector(translate_matrix, input_v); //矩阵作用到每个顶点
			(*iter).x = input_v.dx;
			(*iter).y = input_v.dy;
			(*iter).z = input_v.dz;
		}
	}
}

void my_rotate_x_homogeneous(float angle)
{
	//装配生成旋转矩阵 
	float translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(int) * 9);
	float PAI = 3.14159;
	angle = angle * PAI / 180;
	translate_matrix[0][0] = 1;
	translate_matrix[1][1] = cos(angle);
	translate_matrix[1][2] = -sin(angle);
	translate_matrix[2][1] = sin(angle);
	translate_matrix[2][2] = cos(angle);

	//遍历多边形每个顶点
	for (int i = 0; i < 10; i++)
	{
		list<my_homogeneous_point>::iterator iter = model[i].mList.begin();
		for (; iter != model[i].mList.end(); iter++)
		{
			struct my_3Dvector input_v;
			input_v.dx = (*iter).x;
			input_v.dy = (*iter).y;
			input_v.dz = (*iter).z;
			input_v = matrix_multiply_vector(translate_matrix, input_v); //矩阵作用到每个顶点
			(*iter).x = input_v.dx;
			(*iter).y = input_v.dy;
			(*iter).z = input_v.dz;
		}
	}
}

void my_rotate_y_zhc(zhenhuancun& zhc_, float angle)
{
	//装配生成旋转矩阵 
	float translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(int) * 9);
	float PAI = 3.14159;
	angle = angle * PAI / 180;
	translate_matrix[1][1] = 1;
	translate_matrix[0][0] = cos(angle);
	translate_matrix[0][2] = sin(angle);
	translate_matrix[2][0] = -sin(angle);
	translate_matrix[2][2] = cos(angle);

	//遍历多边形每个顶点

	struct my_3Dvector input_v;
	input_v.dx = zhc_.x;
	input_v.dy = zhc_.y;
	input_v.dz = zhc_.z;
	input_v = matrix_multiply_vector(translate_matrix, input_v); //矩阵作用到每个顶点
	zhc_.x = input_v.dx;
	zhc_.y = input_v.dy;
	zhc_.z = input_v.dz;
}

void my_rotate_x_zhc(zhenhuancun& zhc_, float angle)
{
	//装配生成旋转矩阵 
	float translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(int) * 9);
	float PAI = 3.14159;
	angle = angle * PAI / 180;
	translate_matrix[0][0] = 1;
	translate_matrix[1][1] = cos(angle);
	translate_matrix[1][2] = -sin(angle);
	translate_matrix[2][1] = sin(angle);
	translate_matrix[2][2] = cos(angle);

	//遍历多边形每个顶点

	struct my_3Dvector input_v;
	input_v.dx = zhc_.x;
	input_v.dy = zhc_.y;
	input_v.dz = zhc_.z;
	input_v = matrix_multiply_vector(translate_matrix, input_v); //矩阵作用到每个顶点
	zhc_.x = input_v.dx;
	zhc_.y = input_v.dy;
	zhc_.z = input_v.dz;
}

void worldtolocal(coordinate_system local)
{
	/*float PAI = 3.14159;
	//函数原型：double atan2(double y,double x),返回弧度制表示的y/x的反正切
	float angle1 = atan2(local.z.dz, local.z.dx) * 180 / PAI;
	float angle2 = atan2(local.z.dy, sqrt(local.z.dx * local.z.dx + local.z.dz * local.z.dz)) * 180 / PAI;	//向量与面的夹角求法

	my_rotate_x_homogeneous(angle2);
	my_rotate_y_homogeneous(-angle1);*/

	float angle1 = atan2(eye_y, eye_z) * 180 / pi;
	float angle2 = atan2(eye_x, sqrt(eye_y * eye_y + eye_z * eye_z)) * 180 / pi;

	//a,b,c转到z轴
	my_rotate_x_homogeneous(angle1);
	my_rotate_y_homogeneous(-angle2);
}

void localtoworld(zhenhuancun& zhc_, coordinate_system local)
{
	/*float PAI = 3.14159;
	//函数原型：double atan2(double y,double x),返回弧度制表示的y/x的反正切
	float angle1 = atan2(local.z.dz, local.z.dx) * 180 / PAI;
	float angle2 = atan2(local.z.dy, sqrt(local.z.dx * local.z.dx + local.z.dz * local.z.dz)) * 180 / PAI;	//向量与面的夹角求法

	my_rotate_y_zhc(zhc_, angle1);		//将旋转轴绕Y轴逆时针旋转至平面YOZ
	my_rotate_x_zhc(zhc_, -angle2);	 //将旋转轴绕X轴顺时针旋转至Z轴*/

	float angle1 = atan2(eye_y, eye_z) * 180 / pi;
	float angle2 = atan2(eye_x, sqrt(eye_y * eye_y + eye_z * eye_z)) * 180 / pi;

	//a,b,c转到z轴
	my_rotate_y_zhc(zhc_, angle2);
	my_rotate_x_zhc(zhc_, -angle1);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
bool pointinface(int x, int y, my_face_homogeneous face)
{
	list<my_homogeneous_point>::iterator it, iter;

	//判断象限
	for (iter = face.mList.begin(); iter != face.mList.end(); iter++)
	{
		if ((iter->x > x || iter->x == 0) && iter->y > y)
			iter->xiangxian = 1;
		else if (iter->x > x && (iter->y < y || iter->y == 0))
			iter->xiangxian = 2;
		else if ((iter->x < x || iter->x == 0) && iter->y < y)
			iter->xiangxian = 3;
		else if (iter->x < x && (iter->y > y || iter->y == 0))
			iter->xiangxian = 4;
		else if (iter->x == x && iter->y == y)
			return false;
	}

	//计算角度和
	float angle = 0;
	iter = face.mList.begin();
	while (1)
	{
		it = iter;
		iter++;

		if (iter == face.mList.end())
			break;

		switch (it->xiangxian)
		{
		case 1:
		{
			switch (iter->xiangxian)
			{
			case 2:
			{
				angle += pi / 2;
				break;
			}
			case 4:
			{
				angle -= pi / 2;
				break;
			}
			case 3:
			{
				//(y2-y1)(x1-x)-(x2-x1)(y1-y) > = < 0
				float guoling = (iter->y - it->y) * (it->x - x) - (iter->x - it->x) * (iter->y - y);
				if (guoling == 0)
					return false;
				else if (guoling < 0)
					angle -= pi;
				else
					angle += pi;
				/*if (it->x == 0 && iter->x == 0)
					return false;
				else if ((it->y - iter->y) / (it->x - iter->x) > 1)
					angle -= pi;
				else if ((it->y - iter->y) / (it->x - iter->x) < 1)
					angle += pi;*/
				break;
			}
			}
			break;
		}
		case 2:
		{
			switch (iter->xiangxian)
			{
			case 3:
			{
				angle += pi / 2;
				break;
			}
			case 1:
			{
				angle -= pi / 2;
				break;
			}
			case 4:
			{
				float guoling = (iter->y - it->y) * (it->x - x) - (iter->x - it->x) * (iter->y - y);
				if (guoling == 0)
					return false;
				else if (guoling < 0)
					angle -= pi;
				else
					angle += pi;
				/*if (it->y == 0 && iter->y == 0)
					return false;
				else if ((it->y - iter->y) / (it->x - iter->x) < -1)
					angle -= pi;
				else if ((it->y - iter->y) / (it->x - iter->x) > -1)
					angle += pi;*/
				break;
			}
			}
			break;
		}
		case 3:
		{
			switch (iter->xiangxian)
			{
			case 4:
			{
				angle += pi / 2;
				break;
			}
			case 2:
			{
				angle -= pi / 2;
				break;
			}
			case 1:
			{
				float guoling = (iter->y - it->y) * (it->x - x) - (iter->x - it->x) * (iter->y - y);
				if (guoling == 0)
					return false;
				else if (guoling > 0)
					angle -= pi;
				else
					angle += pi;
				/*if (it->x == 0 && iter->x == 0)
					return false;
				else if ((it->y - iter->y) / (it->x - iter->x) < 1)
					angle -= pi;
				else if ((it->y - iter->y) / (it->x - iter->x) > 1)
					angle += pi;*/
				break;
			}
			}
			break;
		}
		case 4:
		{
			switch (iter->xiangxian)
			{
			case 1:
			{
				angle += pi / 2;
				break;
			}
			case 3:
			{
				angle -= pi / 2;
				break;
			}
			case 2:
			{
				float guoling = (iter->y - it->y) * (it->x - x) - (iter->x - it->x) * (iter->y - y);
				if (guoling == 0)
					return false;
				else if (guoling > 0)
					angle -= pi;
				else
					angle += pi;
				/*if (it->y == 0 && iter->y == 0)
					return false;
				else if ((it->y - iter->y) / (it->x - iter->x) > -1)
					angle -= pi;
				else if ((it->y - iter->y) / (it->x - iter->x) < -1)
					angle += pi;*/
				break;
			}
			}
			break;
		}
		}
	}

	iter = face.mList.begin();
	switch (it->xiangxian)
	{
	case 1:
	{
		switch (iter->xiangxian)
		{
		case 2:
		{
			angle += pi / 2;
			break;
		}
		case 4:
		{
			angle -= pi / 2;
			break;
		}
		case 3:
		{
			//(y2-y1)(x1-x)-(x2-x1)(y1-y) > = < 0
			float guoling = (iter->y - it->y) * (it->x - x) - (iter->x - it->x) * (iter->y - y);
			if (guoling == 0)
				return false;
			else if (guoling < 0)
				angle -= pi;
			else
				angle += pi;
			/*if (it->x == 0 && iter->x == 0)
				return false;
			else if ((it->y - iter->y) / (it->x - iter->x) > 1)
				angle -= pi;
			else if ((it->y - iter->y) / (it->x - iter->x) < 1)
				angle += pi;*/
			break;
		}
		}
		break;
	}
	case 2:
	{
		switch (iter->xiangxian)
		{
		case 3:
		{
			angle += pi / 2;
			break;
		}
		case 1:
		{
			angle -= pi / 2;
			break;
		}
		case 4:
		{
			float guoling = (iter->y - it->y) * (it->x - x) - (iter->x - it->x) * (iter->y - y);
			if (guoling == 0)
				return false;
			else if (guoling < 0)
				angle -= pi;
			else
				angle += pi;
			/*if (it->y == 0 && iter->y == 0)
				return false;
			else if ((it->y - iter->y) / (it->x - iter->x) < -1)
				angle -= pi;
			else if ((it->y - iter->y) / (it->x - iter->x) > -1)
				angle += pi;*/
			break;
		}
		}
		break;
	}
	case 3:
	{
		switch (iter->xiangxian)
		{
		case 4:
		{
			angle += pi / 2;
			break;
		}
		case 2:
		{
			angle -= pi / 2;
			break;
		}
		case 1:
		{
			float guoling = (iter->y - it->y) * (it->x - x) - (iter->x - it->x) * (iter->y - y);
			if (guoling == 0)
				return false;
			else if (guoling > 0)
				angle -= pi;
			else
				angle += pi;
			/*if (it->x == 0 && iter->x == 0)
				return false;
			else if ((it->y - iter->y) / (it->x - iter->x) < 1)
				angle -= pi;
			else if ((it->y - iter->y) / (it->x - iter->x) > 1)
				angle += pi;*/
			break;
		}
		}
		break;
	}
	case 4:
	{
		switch (iter->xiangxian)
		{
		case 1:
		{
			angle += pi / 2;
			break;
		}
		case 3:
		{
			angle -= pi / 2;
			break;
		}
		case 2:
		{
			float guoling = (iter->y - it->y) * (it->x - x) - (iter->x - it->x) * (iter->y - y);
			if (guoling == 0)
				return false;
			else if (guoling > 0)
				angle -= pi;
			else
				angle += pi;
			/*if (it->y == 0 && iter->y == 0)
				return false;
			else if ((it->y - iter->y) / (it->x - iter->x) > -1)
				angle -= pi;
			else if ((it->y - iter->y) / (it->x - iter->x) < -1)
				angle += pi;*/
			break;
		}
		}
		break;
	}
	}

	if (angle > 0.5)
		return true;
	else
		return false;
}

float depth(int x, int y, my_face_homogeneous face)
{
	float z;

	int x1, x2, x3, y1, y2, y3, z1, z2, z3;
	int A, B, C, D;
	list<my_homogeneous_point>::iterator iter = face.mList.begin();

	x1 = (*iter).x;
	y1 = (*iter).y;
	z1 = (*iter).z;
	iter++;

	x2 = (*iter).x;
	y2 = (*iter).y;
	z2 = (*iter).z;
	iter++;

	x3 = (*iter).x;
	y3 = (*iter).y;
	z3 = (*iter).z;

	A = (y3 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1);
	B = (x3 - x1) * (z2 - z1) - (x2 - x1) * (z3 - z1);
	C = (x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1);
	D = -(A * x1 + B * y1 + C * z1);

	z = -(A * x + B * y + D) / static_cast<float>(C);

	return z;
}

bool is_point_In(int i, float x, float y)  //i是第i个面   x，y是需要检测的像素点，这个函数需要My_3DVector头文件 在面内输出true  面的信息用model中的
{
	struct my_homogeneous_point input_point;
	input_point.x = x;
	input_point.y = y;
	input_point.z = 0;
	int point_count = 0; //还是等于1？
	list<my_homogeneous_point>::iterator iter = model[i].mList.begin();
	for (; iter != model[i].mList.end(); iter++)
	{
		point_count++;
	}
	//step 1：逆时针顺序生成点和多边形构成的向量
	my_3Dvector* generated_vectors = new my_3Dvector[point_count];//用于存储所有生成的向量input_point指向每个顶点	
	iter = model[i].mList.begin();
	for (int vIndex = 0; vIndex < point_count; vIndex++)
	{
		//my_3Dvector temp_vector(input_point, iter);
		///////////////////////////////////////////////////////////////////////////////////////////////////
		my_homogeneous_point ZweiLing;
		ZweiLing.x = (*iter).x;
		ZweiLing.y = (*iter).y;
		ZweiLing.z = 0;
		my_3Dvector temp_vector(input_point, ZweiLing);
		///////////////////////////////////////////////////////////////////////////////////////////////////
		//my_3Dvector temp_vector(input_point, *iter);
		generated_vectors[vIndex] = temp_vector;
		iter++;
	}

	//step 2：计算多边形每条边对应的角度，即每两条相邻向量之间形成的夹角，的总和（带有方向）
	//		//此处假定投影面朝向为（0,0,1）——根据实际情况设置
	my_3Dvector face_normal(0, 0, 1);
	float sigma_total_angle = 0;
	for (int eindex = 0; eindex < point_count; eindex++)
	{
		//计算多边形每条边对应的角度，即每两条相邻向量之间形成的夹角，无正负
		int start_vec_index = eindex % point_count;
		int end_vec_index = (eindex + 1) % point_count;
		float dot_val = generated_vectors[start_vec_index].dot_multiply(generated_vectors[end_vec_index]);
		float costheta = dot_val / (generated_vectors[start_vec_index].len * generated_vectors[end_vec_index].len);
		float theta = acosf(costheta) * 180 / 3.1415926535;

		//计算相邻两条向量形成角度属于逆时针（+）还是顺时针（-）-》与投影面朝向做点乘，若大于0，则同为逆时针取﹢，否则取﹣
		my_3Dvector cross_vector = generated_vectors[start_vec_index].cross_multiply(generated_vectors[end_vec_index]);
		float sign = cross_vector.dot_multiply(face_normal) > 0 ? 1 : -1;

		sigma_total_angle += sign * theta;
	}

	delete[] generated_vectors;
	//step 3：根据累积角sigma_total_angle，确定给定点在多边形的内还是外
	//考虑到浮点数舍入误差, fabs(x)<=1e-4,认为x=0；1e-4即小数点后6位
	if (sigma_total_angle > 1e-4) return true;
	else return false;
	//return fabsf(sigma_total_angle) > 1e-4 ? true : false;
}

void z_Buffer()
{
	//备份
	model2 = model;

	//局部坐标系初始化
	//jubu.z.dx = eye_x;
	//jubu.z.dy = eye_y;
	//jubu.z.dz = eye_z;

	//jubu.init();

	/*my_axis_rotate_homogeneous(jubu.x, eye_x, eye_y, eye_z);
	my_axis_rotate_homogeneous(jubu.y, eye_x, eye_y, eye_z);
	my_axis_rotate_homogeneous(jubu.z, eye_x, eye_y, eye_z);*/

	//转换到局部坐标系
	worldtolocal(jubu);
	xiaoyin();

	//帧缓存重置
	for (int i = 0; i < bianchang; i++)
		for (int j = 0; j < bianchang; j++)
		{
			zhc[i][j].x = i - (bianchang - 1) / 2;
			zhc[i][j].y = j - (bianchang - 1) / 2;
			zhc[i][j].z = 0;
			zhc[i][j].zb = -200;
			zhc[i][j].r = 0;
			zhc[i][j].g = 0;
			zhc[i][j].b = 0;
		}

	//z_Buffer
	float cur_depth;
	for (int k = 0; k < 10; k++)//每个面
		if (model[k].n.dot_multiply(v) > 0)//背面剔除
			for (int i = 0; i < bianchang; i++)
				for (int j = 0; j < bianchang; j++)
					if (is_point_In(k, i, j) && (cur_depth = depth(i, j, model[k])) > zhc[i][j].zb)
						//if (pointinface(i, j, model[k]) && (cur_depth = depth(i, j, model[k])) > zhc[i][j].zb)
					{
						zhc[i][j].zb = cur_depth;
						switch (k)
						{
						case 0:
						{
							zhc[i][j].r = 1;
							zhc[i][j].g = 1;
							zhc[i][j].b = 1;
							break;
						}
						case 1:
						{
							zhc[i][j].r = 1;
							zhc[i][j].g = 0;
							zhc[i][j].b = 0;
							break;
						}
						case 2:
						{
							zhc[i][j].r = 0;
							zhc[i][j].g = 1;
							zhc[i][j].b = 0;
							break;
						}
						case 3:
						{
							zhc[i][j].r = 0;
							zhc[i][j].g = 0;
							zhc[i][j].b = 1;
							break;
						}
						case 4:
						{
							zhc[i][j].r = 1;
							zhc[i][j].g = 1;
							zhc[i][j].b = 0;
							break;
						}
						case 5:
						{
							zhc[i][j].r = 1;
							zhc[i][j].g = 0;
							zhc[i][j].b = 1;
							break;
						}
						case 6:
						{
							zhc[i][j].r = 0;
							zhc[i][j].g = 1;
							zhc[i][j].b = 1;
							break;
						}
						case 7:
						{
							zhc[i][j].r = 0.5;
							zhc[i][j].g = 0;
							zhc[i][j].b = 0;
							break;
						}
						case 8:
						{
							zhc[i][j].r = 0;
							zhc[i][j].g = 0.5;
							zhc[i][j].b = 0;
							break;
						}
						case 9:
						{
							zhc[i][j].r = 0;
							zhc[i][j].g = 0;
							zhc[i][j].b = 0.5;
							break;
						}
						}
					}

	/*glBegin(GL_POINTS);
	for (int i = 0; i < bianchang; i++)
		for (int j = 0; j < bianchang; j++)
		{
			glColor3f(zhc[i][j].r, zhc[i][j].g, zhc[i][j].b);
			glVertex3f(zhc[i][j].x, zhc[i][j].y, zhc[i][j].z);
		}
	glEnd();*/

	//帧缓存变换到世界坐标系
	for (int i = 0; i < bianchang; i++)
		for (int j = 0; j < bianchang; j++)
			localtoworld(zhc[i][j], jubu);

	//画!
	glBegin(GL_POINTS);
	for (int i = 0; i < bianchang; i++)
		for (int j = 0; j < bianchang; j++)
		{
			glColor3f(zhc[i][j].r, zhc[i][j].g, zhc[i][j].b);
			glVertex3f(zhc[i][j].x, zhc[i][j].y, zhc[i][j].z);
		}
	glEnd();

	//还原
	model = model2;
}

//绘制内容
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	z_Buffer();

	draw_coordinate(); //绘制坐标系 

	glutSwapBuffers();
}

//投影方式、modelview方式设置
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height *
			(GLfloat)nearplane_height / (GLfloat)nearplane_width, 0.5 * nearplane_height *
			(GLfloat)nearplane_height / (GLfloat)nearplane_width,
			-nearplane_distance, farplane_distance); //相对于视点
	else
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height *
			(GLfloat)nearplane_width / (GLfloat)nearplane_height, 0.5 * nearplane_height *
			(GLfloat)nearplane_width / (GLfloat)nearplane_height,
			-nearplane_distance, farplane_distance);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//gluLookAt(eye_x, eye_y, eye_z, 0, 0, 0, 0, 1, 0);
	gluLookAt(0, 0, eye_z, 0, 0, 0, 0, 1, 0);
}

//键盘交互事件
void keyboard(unsigned char key, int x, int y)
{
	float fangda = 1.1;
	switch (key)
	{
	case'1':
	{
		for (vector<my_face_homogeneous>::iterator it = model.begin(); it != model.end(); it++)
			for (list<my_homogeneous_point>::iterator iter = it->mList.begin(); iter != it->mList.end(); iter++)
			{
				iter->x *= fangda;
				iter->y *= fangda;
				iter->z *= fangda;
			}
		glutPostRedisplay();
		break;
	}
	case'2':
	{
		for (vector<my_face_homogeneous>::iterator it = model.begin(); it != model.end(); it++)
			for (list<my_homogeneous_point>::iterator iter = it->mList.begin(); iter != it->mList.end(); iter++)
			{
				iter->x /= fangda;
				iter->y /= fangda;
				iter->z /= fangda;
			}
		glutPostRedisplay();
		break;
	}
	case'a':
	{
		eye_x = eye_x * cosf(-theta) + eye_z * sinf(-theta);
		eye_z = eye_z * cosf(-theta) - eye_x * sinf(-theta);
		v.dx = eye_x;
		v.dy = eye_y;
		v.dz = eye_z;
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case'd':
	{
		eye_x = eye_x * cosf(theta) + eye_z * sinf(theta);
		eye_z = eye_z * cosf(theta) - eye_x * sinf(theta);
		v.dx = eye_x;
		v.dy = eye_y;
		v.dz = eye_z;
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case'w':
	{
		eye_y = eye_y * cosf(theta) + eye_z * sinf(theta);
		eye_z = eye_z * cosf(theta) - eye_y * sinf(theta);
		v.dx = eye_x;
		v.dy = eye_y;
		v.dz = eye_z;
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case's':
	{
		eye_y = eye_y * cosf(-theta) + eye_z * sinf(-theta);
		eye_z = eye_z * cosf(-theta) - eye_y * sinf(-theta);
		v.dx = eye_x;
		v.dy = eye_y;
		v.dz = eye_z;
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case'q':
	{
		eye_x = eye_x * cosf(-theta) + eye_y * sinf(-theta);
		eye_y = eye_y * cosf(-theta) - eye_x * sinf(-theta);
		v.dx = eye_x;
		v.dy = eye_y;
		v.dz = eye_z;
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case'e':
	{
		eye_x = eye_x * cosf(theta) + eye_y * sinf(theta);
		eye_y = eye_y * cosf(theta) - eye_x * sinf(theta);
		v.dx = eye_x;
		v.dy = eye_y;
		v.dz = eye_z;
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case 27:
	{
		exit(0);
		break;
	}
	}
}

//鼠标交互事件
//鼠标点击改变视点方向
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			eye_x = eye_x * cosf(-theta) + eye_z * sinf(-theta);
			eye_z = eye_z * cosf(-theta) - eye_x * sinf(-theta);
			v.dx = eye_x;
			v.dy = 1;
			v.dz = eye_z;
			reshape(nearplane_width, nearplane_height);
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			eye_x = eye_x * cosf(theta) + eye_z * sinf(theta);
			eye_z = eye_z * cosf(theta) - eye_x * sinf(theta);
			v.dx = eye_x;
			v.dy = 1;
			v.dz = eye_z;
			reshape(nearplane_width, nearplane_height);
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

//主调函数
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(nearplane_width, nearplane_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("背面剔除算法");

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}