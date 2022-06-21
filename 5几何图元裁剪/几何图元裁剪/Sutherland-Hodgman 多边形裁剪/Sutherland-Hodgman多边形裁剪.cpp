#define GLUT_DISABLE_ATEXIT_HACK
#include<list>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "My_3DVector.h"
#include "GLUT.H"
using namespace std;

int nearplane_width = 600; //视景体宽度
int nearplane_height = 600; //视景体高度
int nearplane_distance = 50; //视景体近平面与视点距离
int farplane_distance = nearplane_distance + 300; //视景体远平面与视点距离

typedef list<my_homogeneous_point_EX> LISTINT;
//用list容器处理整型数据 
//用LISTINT创建一个名为ListA,ListB的list对象
LISTINT ListA, ListB;
//声明iter为迭代器
LISTINT::iterator iter;

#define clip_polygon_point_count 4 //裁剪多边形顶点数
#define subject_polygon_point_count 4 //被裁剪多边形顶点数
struct my_homogeneous_point_EX subject_polygon[subject_polygon_point_count]; //被裁剪多边形
struct my_homogeneous_point_EX clip_polygon[clip_polygon_point_count]; //裁剪多边形

my_homogeneous_point_EX clip_cur_point;//裁剪边的起点
my_homogeneous_point_EX clip_next_point;//裁剪边的终点
my_homogeneous_point_EX subject_cur_point;//被裁边的起点
my_homogeneous_point_EX subject_next_point;//被裁减边的终点
my_homogeneous_point_EX clip_cross_subject;//裁剪边和被裁减边的交点

//初始化裁剪多边形、被裁减多边形顶点坐标 将被裁减多边形的顶点赋值到ListB中
void init(void)
{
	//被裁减多边形的顶点设置 {0,40},{0,0},{20,20},{65,20},{60,45},{30,45},{5,35}
	subject_polygon[0].x = -90;
	subject_polygon[0].y = 0;
	subject_polygon[0].z = 0;

	subject_polygon[1].x = 180;
	subject_polygon[1].y = 0;
	subject_polygon[1].z = 0;

	subject_polygon[2].x = 180;
	subject_polygon[2].y = 80;
	subject_polygon[2].z = 0;

	subject_polygon[3].x = -90;
	subject_polygon[3].y = 80;
	subject_polygon[3].z = 0;

	//裁剪多边形的顶点设置 {-10,50},{10,30},{-5,10},{40,15},{80,35},{30,80}
	clip_polygon[0].x = 40;
	clip_polygon[0].y = -60;
	clip_polygon[0].z = 0;
	clip_polygon[0].ratio = 1;

	clip_polygon[1].x = 150;
	clip_polygon[1].y = -60;
	clip_polygon[1].z = 0;
	clip_polygon[1].ratio = 1;

	clip_polygon[2].x = 150;
	clip_polygon[2].y = 120;
	clip_polygon[2].z = 0;
	clip_polygon[2].ratio = 1;

	clip_polygon[3].x = 40;
	clip_polygon[3].y = 120;
	clip_polygon[3].z = 0;
	clip_polygon[3].ratio = 1;

	//给ListB赋值
	for (int k = 0; k < subject_polygon_point_count; k++) {
		ListB.push_back(subject_polygon[k]);
	}
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

/***************************************
* 两条边求交点函数
* a1，a2为一条边上的两个端点 b1，b2为另一条边上的两个端点
******************************************/
struct my_homogeneous_point_EX GetCrossPoint(my_homogeneous_point_EX a1, my_homogeneous_point_EX a2, my_homogeneous_point_EX b1, my_homogeneous_point_EX b2)
{
	float A1 = a2.y - a1.y;
	float B1 = a1.x - a2.x;
	float C1 = a2.x * a1.y - a1.x * a2.y;
	float A2 = b2.y - b1.y;
	float B2 = b1.x - b2.x;
	float C2 = b2.x * b1.y - b1.x * b2.y;
	my_homogeneous_point_EX cross;
	cross.x = (C2 * B1 - C1 * B2) / (A1 * B2 - A2 * B1);
	cross.y = (A1 * C2 - A2 * C1) / (A2 * B1 - A1 * B2);
	return cross;
}

/***************************************
* 判断给定点在给定裁剪边的可见侧还是不可见侧
* edge_start_point，edge_end_point为当前裁剪边的起点和终点；given_point为给定点
* clip_polygon为裁剪多边形
******************************************/
bool determine_point_edge_position(my_homogeneous_point_EX edge_start_point, my_homogeneous_point_EX edge_end_point, my_homogeneous_point_EX given_point)
{
	//求出裁剪平面上相邻的两个向量，计算投影面的法向量
	my_3Dvector a(clip_polygon[1], clip_polygon[2]);
	my_3Dvector b(clip_polygon[1], clip_polygon[3]);
	my_3Dvector face_normal = a.cross_multiply(b);

	//计算相邻两个向量形成角度属于逆时针(+)还是顺时针(-)——与裁剪平面法向做点乘，若大于0，则同为逆时针取﹢，否则取﹣
	my_3Dvector c(edge_start_point, edge_end_point);//c为裁剪线段向量
	my_3Dvector d(edge_start_point, given_point);//d为判断点与裁剪线段
	my_3Dvector cross_vector = c.cross_multiply(d);

	float sign = cross_vector.dot_multiply(face_normal);
	return sign >= 1e-6;//在可见侧返回1，在不可见侧返回0；
}

/***************************************
* 裁剪函数Sutherland_Hodgman_Clipping
* clip_polygon，subject_polygon分别为裁剪多边形和被裁剪多边形
* clip_polygon_point_count表示裁剪多边形顶点个数
* subject_polygon_point_count表示被裁剪多边形顶点个数
******************************************/
void Sutherland_Hodgman_Clipping()
{
	for (int i = 0; i < clip_polygon_point_count; i++)
	{
		//第一步:取出当前点clip_cur_point和下一点clip_next-point
		ListA.clear(); // ListA为链表用于存储每条边的裁剪结果
		int start_vec_index = i % clip_polygon_point_count;
		int end_vec_index = (i + 1) % clip_polygon_point_count;
		clip_cur_point = clip_polygon[start_vec_index];
		clip_next_point = clip_polygon[end_vec_index];
		bool flag = true;

		//第二步:遍历subject_polygon，取出当前点subject_cur_point和下一点subject_next_point,
			for (iter = ListB.begin(); flag == true; iter++)
			{
				subject_cur_point = *iter;
				iter++;
				if (iter == ListB.end())
				{
					flag = false;
					iter = ListB.begin();
					subject_next_point = *iter;
				}
				else
				{
					subject_next_point = *iter;
				}
				if (iter != ListB.begin())
				{
					iter--;
				}

				bool a = determine_point_edge_position(clip_cur_point, clip_next_point, subject_cur_point);
				bool b = determine_point_edge_position(clip_cur_point, clip_next_point, subject_next_point);

				//根据规则，进行边求交或顶点取舍，如有点需要保留则插入到ListA中
				if (a == false && b == true)
				{
					clip_cross_subject = GetCrossPoint(clip_cur_point, clip_next_point, subject_cur_point, subject_next_point);
					ListA.push_back(clip_cross_subject);
					ListA.push_back(subject_next_point);
				}
				else if (a == true && b == true)
				{
					//都在可见侧 储存终点
					ListA.push_back(subject_next_point);
				}
				else if (a == true && b == false)
				{
					//起点在可见侧，终点在不可见侧 储存交点
					clip_cross_subject = GetCrossPoint(clip_cur_point, clip_next_point, subject_cur_point, subject_next_point);
					ListA.push_back(clip_cross_subject);
				}
			}
		ListB.clear();
		//第三步：将ListA复制到ListB数组，再回到第一步
		//ListB存储最终的裁剪结果
		for (iter = ListA.begin(); iter != ListA.end(); iter++)
		{
			ListB.push_back(*iter);
		}
	}
}

//键盘交互事件
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	case 'W':
	{
		Sutherland_Hodgman_Clipping();
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

	glLineWidth(3);//设置线的宽度为3
	draw_coordinate(); //绘制坐标系

	glColor3f(0, 0, 0);
	//绘制clipping_polygon,默认顶点之间通过直线段相连 绘制裁剪多边形
	glBegin(GL_LINES);
	for (int vindex = 0; vindex < clip_polygon_point_count; vindex++)
	{
		int start_p_index = vindex % clip_polygon_point_count;
		int end_p_index = (vindex + 1) % clip_polygon_point_count;
		glVertex3i((floor)(clip_polygon[start_p_index].x + 0.5),
			(floor)(clip_polygon[start_p_index].y + 0.5), (floor)(clip_polygon[start_p_index].z + 0.5));
		glVertex3i((floor)(clip_polygon[end_p_index].x + 0.5),
			(floor)(clip_polygon[end_p_index].y + 0.5), (floor)(clip_polygon[end_p_index].z + 0.5));
	}
	glEnd();

	glColor3f(0, 0, 1);//绘制被裁减多边形
	glBegin(GL_LINE_LOOP);
	for (iter = ListB.begin(); iter != ListB.end(); iter++)
	{
		glVertex3i((floor)((*iter).x + 0.5), (floor)((*iter).y + 0.5), 0);
	}
	glEnd();
	glutSwapBuffers();
}

//主调函数
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(nearplane_width, nearplane_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Sutherland-Hodgman");

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}