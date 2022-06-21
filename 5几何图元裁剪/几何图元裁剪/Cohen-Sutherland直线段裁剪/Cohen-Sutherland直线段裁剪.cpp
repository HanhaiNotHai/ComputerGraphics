#define GLUT_DISABLE_ATEXIT_HACK
#include<list>
#include "GLUT.H"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "My_3DVector.h"
using namespace std;

int nearplane_width = 600; //视景体宽度
int nearplane_height = 600;//视景体高度
int nearplane_distance = 50; //视景体近平面与视点距离
int farplane_distance = nearplane_distance + 300; //视景体远平面与视点距离

typedef list<my_homogeneous_point> LISTINT;
//用list容器处理整型数据 
//用LISTINT创建一个名为List的list对象
LISTINT List;
//声明iter为迭代器
LISTINT::iterator iter;

//测试顶点 直线端点
my_homogeneous_point test_point1;
my_homogeneous_point test_point2;

//裁剪多边形顶点坐标
//每条边都是直线
#define clip_polygon_point_count 4
struct my_homogeneous_point clip_polygon[clip_polygon_point_count]; //裁剪框

//被裁剪多边形顶点坐标
//每条边都是直线
struct my_homogeneous_point subject_polygon[4];

//初始化测试直线以及裁剪框的坐标
void init(void)
{
	//测试点位置初始化
	test_point1.x = 20 * 2;
	test_point1.y = 10 * 2;
	test_point1.z = 0;
	//测试点位置初始化
	test_point2.x = 80 * 2;
	test_point2.y = 50 * 2;
	test_point2.z = 0;

	//裁剪框的设置
	clip_polygon[0].x = 0;
	clip_polygon[0].y = 0;
	clip_polygon[0].z = 0;
	clip_polygon[0].ratio = 1;

	clip_polygon[1].x = 200;
	clip_polygon[1].y = 0;
	clip_polygon[1].z = 0;
	clip_polygon[1].ratio = 1;

	clip_polygon[2].x = 200;
	clip_polygon[2].y = 120;
	clip_polygon[2].z = 0;
	clip_polygon[2].ratio = 1;

	clip_polygon[3].x = 0;
	clip_polygon[3].y = 120;
	clip_polygon[3].z = 0;
	clip_polygon[3].ratio = 1;
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
	glEnd();
}

//矩阵向量相乘
struct my_homogeneous_point matrix_multiply_vector(float matrix[][4], struct my_homogeneous_point input_v)
{
	struct my_homogeneous_point translated_v;
	translated_v.x = matrix[0][0] * input_v.x + matrix[0][1] * input_v.y + matrix[0][2] * input_v.z +matrix[0][3] * input_v.ratio;
	translated_v.y = matrix[1][0] * input_v.x + matrix[1][1] * input_v.y + matrix[1][2] * input_v.z +matrix[1][3] * input_v.ratio;
	translated_v.z = matrix[2][0] * input_v.x + matrix[2][1] * input_v.y + matrix[2][2] * input_v.z +matrix[2][3] * input_v.ratio;
	translated_v.ratio = matrix[3][0] * input_v.x + matrix[3][1] * input_v.y + matrix[3][2] *input_v.z + matrix[3][3] * input_v.ratio;
	return translated_v;
}

//齐次平移变换
void my_traslate_homogeneous(struct my_homogeneous_point* polygon, int point_count, float tx,float ty, float tz)
{
	//装配生成平移矩阵
	float translate_matrix[4][4];
	memset(translate_matrix, 0, sizeof(int) * 16);
	translate_matrix[0][0] = translate_matrix[1][1] = translate_matrix[2][2] = translate_matrix[3][3]= 1;//对角线赋值为1
	translate_matrix[0][3] = tx;
	translate_matrix[1][3] = ty;
	translate_matrix[2][3] = tz;

	//遍历并平移多边形每个顶点
	for (int vIndex = 0; vIndex < point_count; vIndex++)
	{
		struct my_homogeneous_point input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.z = polygon[vIndex].z;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v); //平移矩阵作用到每个顶点
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
		polygon[vIndex].z = input_v.z;
		polygon[vIndex].ratio = input_v.ratio;
	}
}

/***************************************
* 编码函数，输入点坐标，输出4位编码
* px，py为输入点 clip_polygon为裁剪多边形 clip_polygon_point_count为裁剪多边形的顶点数
量
* 4位编码的意义如下（从左到右)：
* 第1位：如果端点在裁剪多边形的左侧，则为1，否则为0
* 第2位：如果端点在裁剪多边形的右侧，则为1，否则为0
* 第3位：如果端点在裁剪多边形的下侧，则为1，否则为0
* 第4位：如果端点在裁剪多边形的上侧，则为1，否则为0
从右到左？
******************************************/
unsigned int enCode(double px, double py)
{
	//用1 2 4 8 编码裁剪框的左、右、下、上区域
	unsigned int RC = 0;
	if (px < clip_polygon[0].x)
	{
		RC = RC | 1;//0001
	}
	else if (px > clip_polygon[1].x)
	{
		RC = RC | 2;//0010
	}
	if (py < clip_polygon[0].y)
	{
		RC = RC | 4;//0100
	}
	else if (py > clip_polygon[3].y)
	{
		RC = RC | 8;//1000
	}
	return RC;
}

//Cohen-Sutherland 裁剪函数
//根据线段的两个端点 point1 和 point2 的编码判断，该线段与裁剪多边形的位置，求出裁剪后的两个端点
void cilpping_lines(my_homogeneous_point point1, my_homogeneous_point point2)
{
	my_homogeneous_point test_point11 = point1;//用test_point11、test_point22储存直线两个端点
	my_homogeneous_point test_point22 = point2;

	float x = 0; //储存变化后的x，y坐标
	float y = 0;

	int code1 = enCode(test_point11.x, test_point11.y); //code1、code2记录顶点的编码 
	int code2 = enCode(test_point22.x, test_point22.y);
	int code = 0;

	while (code1 != 0 || code2 != 0) //两直线至少有一点在区域外
	{
		if ((code1 & code2) != 0)
		{ //两点连线不过裁剪区域,跳出循环
			return;
		}

		//将在区域外的点编码给code
		code = code1;
		if (code1 == 0) code = code2;

		if ((1 & code) != 0) //直线段与左边界相交，求出直线与边界相交点
		{
			x = clip_polygon[0].x;
			y = test_point11.y + (test_point22.y - test_point11.y) * (clip_polygon[0].x -test_point11.x) / (test_point22.x - test_point11.x);
		}
		else if ((2 & code) != 0) //直线段与右边界相交，求出直线与边界相交点
		{
			x = clip_polygon[1].x;
			y = test_point11.y + (test_point22.y - test_point11.y) * (clip_polygon[1].x -test_point11.x) / (test_point22.x - test_point11.x);
		}
		else if ((4 & code) != 0) //直线段与下边界相交，求出直线与边界相交点
		{
			y = clip_polygon[0].y;
			x = test_point11.x + (test_point22.x - test_point11.x) * (clip_polygon[0].y -test_point11.y) / (test_point22.y - test_point11.y);
		}
		else if ((8 & code) != 0) //直线段与上边界相交，求出直线与边界相交点
		{
			y = clip_polygon[3].y;
			x = test_point11.x + (test_point22.x - test_point11.x) * (clip_polygon[3].y -test_point11.y) / (test_point22.y - test_point11.y);
		}

		//将求出的点重新赋值
		if (code == code1)
		{
			test_point11.x = x;
			test_point11.y = y;
			code1 = enCode(x, y);
		}
		else
		{
			test_point22.x = x;
			test_point22.y = y;
			code2 = enCode(x, y);
		}
	}
	//将新生成的点存入List中
	List.push_back(test_point11);
	List.push_back(test_point22);
}

//键盘交互事件
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	case 'W':
	{
		my_traslate_homogeneous(clip_polygon, clip_polygon_point_count, 0, 1, 0);//向Y轴正方向移动1个单元
		glutPostRedisplay();
		break;
	}
	case 's':
	case 'S':
	{
		my_traslate_homogeneous(clip_polygon, clip_polygon_point_count, 0, -1, 0);//向Y轴负方向移动1个单元
		glutPostRedisplay();
		break;
	}
	case 'a':
	case 'A':
	{
		my_traslate_homogeneous(clip_polygon, clip_polygon_point_count, -1, 0, 0);//向X轴负方向移动1个单元
		glutPostRedisplay();
		break;
	}
	case 'd':
	case 'D':
	{
		my_traslate_homogeneous(clip_polygon, clip_polygon_point_count, 1, 0, 0); //向X轴正方向移动1个单元
		glutPostRedisplay();
		break;
	}
	case 27:
		exit(0);
		break;
	}
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
}

//绘制内容
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLineWidth(3);//定义直线宽度
	draw_coordinate(); //绘制坐标系

	glColor3f(157.0 / 256, 195.0 / 256, 230.0 / 256);
	//绘制clipping_polygon,默认顶点之间通过直线段相连 绘制裁剪区域

	glBegin(GL_LINES);
	for (int vindex = 0; vindex < clip_polygon_point_count; vindex++)
	{
		int start_p_index = vindex % clip_polygon_point_count;
		int end_p_index = (vindex + 1) % clip_polygon_point_count;
		glVertex3i((floor)(clip_polygon[start_p_index].x + 0.5),
			(floor)(clip_polygon[start_p_index].y + 0.5), 
			(floor)(clip_polygon[start_p_index].z + 0.5));
		glVertex3i((floor)(clip_polygon[end_p_index].x + 0.5),
			(floor)(clip_polygon[end_p_index].y + 0.5), 
			(floor)(clip_polygon[end_p_index].z + 0.5));
	}
	glEnd();

	cilpping_lines(test_point1, test_point2);

	glBegin(GL_LINES);//绘制被裁减直线
	for (std::list<my_homogeneous_point>::iterator iter = List.begin(); iter != List.end(); iter++)
	{
		glVertex3i((floor)(iter->x + 0.5), (floor)(iter->y + 0.5), (floor)(iter->z + 0.5));
	}
	glEnd();

	List.clear();
	glutSwapBuffers();
}

//主调函数
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(nearplane_width, nearplane_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Cohen-Sutherland直线段裁剪");

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}