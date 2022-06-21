//7.3 课外拓展性实验.cpp
#define GLUT_DISABLE_ATEXIT_HACK
#include <stdlib.h>
#include "glut.h"
#include <math.h>
#include <vector>
#include <map>

#define epsion 1e-3

struct my_v_inhomogeneous
{
	float x;
	float y;
	my_v_inhomogeneous(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
};

//多边形边
struct Edge
{
	float cur_X;//当前扫描线与当前边交点的x坐标值
	float reciprocal_K;//当前边斜率的倒数
	int max_Y;//当前边俩端点最大的y坐标值
	Edge(int y, float x, float rec_k)
	{
		max_Y = y;
		cur_X = x;
		reciprocal_K = rec_k;
	}
};

std::vector<my_v_inhomogeneous> polygonPoints1, polygonPoints2, line;
std::map<int, std::vector<Edge>> ET;
std::vector<Edge> AET;

bool filling = false;
int y_min, y_max;

//绘制坐标系
void draw_coordinate()
{
	glBegin(GL_LINES);
	glVertex2f(-5000, 0.0);
	glVertex2f(5000, 0.0);

	glVertex2f(0.0, -5000);
	glVertex2f(0.0, 5000);
	glEnd();
}

void generate_polygon_points()
{
	my_v_inhomogeneous p01(-100, -200);
	y_min = p01.y;
	y_max = p01.y;
	polygonPoints1.push_back(p01);

	my_v_inhomogeneous p02(-100, 200);
	y_min = p02.y < y_min ? p02.y : y_min;
	y_max = p02.y > y_max ? p02.y : y_max;
	polygonPoints1.push_back(p02);

	my_v_inhomogeneous p03(200, 200);
	y_min = p03.y < y_min ? p03.y : y_min;
	y_max = p03.y > y_max ? p03.y : y_max;
	polygonPoints1.push_back(p03);
	
	my_v_inhomogeneous p04(100, -150);
	y_min = p04.y < y_min ? p04.y : y_min;
	y_max = p04.y > y_max ? p04.y : y_max;
	polygonPoints1.push_back(p04);
	
	my_v_inhomogeneous p05(400, -300);
	y_min = p05.y < y_min ? p05.y : y_min;
	y_max = p05.y > y_max ? p05.y : y_max;
	polygonPoints1.push_back(p05);
	
	my_v_inhomogeneous p06(400, 400);
	y_min = p06.y < y_min ? p06.y : y_min;
	y_max = p06.y > y_max ? p06.y : y_max;
	polygonPoints1.push_back(p06);
	
	my_v_inhomogeneous p07(150, 300);
	y_min = p07.y < y_min ? p07.y : y_min;
	y_max = p07.y > y_max ? p07.y : y_max;
	polygonPoints1.push_back(p07);
	
	my_v_inhomogeneous p08(-100, 400);
	y_min = p08.y < y_min ? p08.y : y_min;
	y_max = p08.y > y_max ? p08.y : y_max;
	polygonPoints1.push_back(p08);
	
	my_v_inhomogeneous p09(-250, 200);
	y_min = p09.y < y_min ? p09.y : y_min;
	y_max = p09.y > y_max ? p09.y : y_max;
	polygonPoints1.push_back(p09);
	
	my_v_inhomogeneous p10(-300, -50);
	y_min = p10.y < y_min ? p10.y : y_min;
	y_max = p10.y > y_max ? p10.y : y_max;
	polygonPoints1.push_back(p10);
	
	my_v_inhomogeneous p11(200, -50);
	y_min = p11.y < y_min ? p11.y : y_min;
	y_max = p11.y > y_max ? p11.y : y_max;
	polygonPoints2.push_back(p11);
	
	my_v_inhomogeneous p12(350, -50);
	y_min = p12.y < y_min ? p12.y : y_min;
	y_max = p12.y > y_max ? p12.y : y_max;
	polygonPoints2.push_back(p12);
	
	my_v_inhomogeneous p13(350, 50);
	y_min = p13.y < y_min ? p13.y : y_min;
	y_max = p13.y > y_max ? p13.y : y_max;
	polygonPoints2.push_back(p13);
	
	my_v_inhomogeneous p14(250, 100);
	y_min = p14.y < y_min ? p14.y : y_min;
	y_max = p14.y > y_max ? p14.y : y_max;
	polygonPoints2.push_back(p14);
}

/***************************************
* 生成多边形的ET表
* polygon：逆时针排列的顶点数组
* 遍历polygon,相邻两顶点之间构造一条边Edge，并同时根据两顶点y坐标最小值，对
所有边进行分类和插入排序
***************************************/
void generate_ET()
{
	for (int eindex = 0; eindex < polygonPoints1.size(); eindex++)
	{
		my_v_inhomogeneous startPoint = polygonPoints1[eindex];
		my_v_inhomogeneous endPoint = (eindex == (polygonPoints1.size() - 1)) ? polygonPoints1[0] : polygonPoints1[eindex + 1];
		
		if (endPoint.y == startPoint.y)
		{
			line.push_back(startPoint);
			line.push_back(endPoint);
			continue;
		}

		my_v_inhomogeneous top_point = startPoint.y > endPoint.y ? startPoint : endPoint;
		my_v_inhomogeneous bottom_point = startPoint.y < endPoint.y ? startPoint : endPoint;
		float rec_k = (top_point.x - bottom_point.x) / (top_point.y - bottom_point.y);
		Edge one_ET_Edge(top_point.y, bottom_point.x, rec_k);
		
		std::map<int, std::vector<Edge>>::iterator it = ET.find(bottom_point.y);
		if (it == ET.end())
		{
			std::vector<Edge> new_edge_list;
			new_edge_list.push_back(one_ET_Edge);
			ET.insert(std::pair<int, std::vector<Edge>>(bottom_point.y, new_edge_list));
		}
		else//sort
		{
			bool inserted = false;
			for (std::vector<Edge>::iterator eit = it->second.begin(); eit != it->second.end(); eit++)
			{
				if (bottom_point.x < eit->cur_X || (bottom_point.x == eit->cur_X && rec_k < eit->reciprocal_K))
				{
					it->second.insert(eit, one_ET_Edge);
					inserted = true;
					break;
				}
			}
			if (false == inserted)
			{
				it->second.push_back(one_ET_Edge);
			}
		}
	}

	for (int eindex = 0; eindex < polygonPoints2.size(); eindex++)
	{
		my_v_inhomogeneous startPoint = polygonPoints2[eindex];
		my_v_inhomogeneous endPoint = (eindex == (polygonPoints2.size() - 1)) ? polygonPoints2[0] : polygonPoints2[eindex + 1];
		
		if (endPoint.y == startPoint.y)
		{
			line.push_back(startPoint);
			line.push_back(endPoint);
			continue;
		}

		my_v_inhomogeneous top_point = startPoint.y > endPoint.y ? startPoint : endPoint;
		my_v_inhomogeneous bottom_point = startPoint.y < endPoint.y ? startPoint : endPoint;
		float rec_k = (top_point.x - bottom_point.x) / (top_point.y - bottom_point.y);
		Edge one_ET_Edge(top_point.y, bottom_point.x, rec_k);
		
		std::map<int, std::vector<Edge>>::iterator it = ET.find(bottom_point.y);
		if (it == ET.end())
		{
			std::vector<Edge> new_edge_list;
			new_edge_list.push_back(one_ET_Edge);
			ET.insert(std::pair<int, std::vector<Edge>>(bottom_point.y, new_edge_list));
		}
		else
		{
			bool inserted = false;
			for (std::vector<Edge>::iterator eit = it->second.begin(); eit != it->second.end(); eit++)
			{
				if (bottom_point.x < eit->cur_X || (bottom_point.x == eit->cur_X && rec_k < eit->reciprocal_K))
				{
					it->second.insert(eit, one_ET_Edge);
					inserted = true;
					break;
				}
			}
			if (false == inserted)
			{
				it->second.push_back(one_ET_Edge);
			}
		}
	}
}

/***************************************
* 往AET中插入新的Edge集合，并排序
* polygon：逆时针排列的顶点数组
***************************************/
void insert_new_edges_into_AET(std::vector<Edge> newedges)
{
	//根据AET中边的排序规则，对每条边进行插入排序
	for (std::vector<Edge>::iterator newHead = newedges.begin(); newHead != newedges.end(); newHead++)
	{
		bool inserted = false;
		for (std::vector<Edge>::iterator AET_Head = AET.begin(); AET_Head != AET.end(); AET_Head++)
		{
			if (newHead->cur_X < AET_Head->cur_X || (newHead->cur_X == AET_Head->cur_X && newHead->reciprocal_K < AET_Head->reciprocal_K))
			{
				AET.insert(AET_Head, *newHead);
				inserted = true;
				break;
			}
		}
		if (false == inserted)
		{
			AET.push_back(*newHead);
		}
	}
}

/***************************************
* 扫描线填充
***************************************/
void scanline_filling()
{
	AET.clear();
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int ypos = y_min; ypos <= y_max; ypos += 1)//逐行遍历所有扫描线
	{
		std::map<int, std::vector<Edge>>::iterator it = ET.find(ypos);
		if (it != ET.end())
		{
			insert_new_edges_into_AET(it->second);//插入到AET表中
		}
		std::vector<int> need_removed_edge_index;//存放需要删除的边序号
		glBegin(GL_LINES);//确定填充线段区域并填充
		for (int eindex = 0; eindex < AET.size(); eindex += 2)
		{
			if (AET[eindex].reciprocal_K > 1)
			{
				glVertex2f(AET[eindex].cur_X + 2, ypos);
			}
			else
			{
				glVertex2f(AET[eindex].cur_X + 1, ypos);
			}
			AET[eindex].cur_X += 1 * AET[eindex].reciprocal_K;
			glVertex2f(AET[eindex + 1].cur_X - 1, ypos);
			AET[eindex + 1].cur_X += 1 * AET[eindex + 1].reciprocal_K;
			if (AET[eindex].max_Y == ypos + 1)
				need_removed_edge_index.push_back(eindex);
			if (AET[eindex + 1].max_Y == ypos + 1)
				need_removed_edge_index.push_back(eindex + 1);
		}
		glEnd();
		for (int eindex = need_removed_edge_index.size() - 1; eindex >= 0; eindex--)
		{
			AET.erase(AET.begin() + need_removed_edge_index[eindex]);
		}
	}
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	for (int index = 0; index < line.size(); index++)
	{
		glVertex2f(line[index].x, line[index].y);
	}
	glEnd();
}

void display()
{
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//绘制图形
	glColor3f(0.0f, 1.0f, 0.0f);
	draw_coordinate();

	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	for (int index = 0; index < polygonPoints1.size(); index++)
	{
		glVertex2f(polygonPoints1[index].x, polygonPoints1[index].y);
	}
	glEnd();

	glBegin(GL_LINE_LOOP);
	for (int index = 0; index < polygonPoints2.size(); index++)
	{
		glVertex2f(polygonPoints2[index].x, polygonPoints2[index].y);
	}
	glEnd();

	if (filling == true)
	{
		scanline_filling();
	}
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h); //它负责把视景体截取的场景按照怎样的高和宽显示到屏幕上
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-0.5 * w, 0.5 * w, -0.5 * h, 0.5 * h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard_callback(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		filling = true;
		glutPostRedisplay();
		break;
	case '2':
		filling = false;
		glutPostRedisplay();
		break;
	case 27:
		exit(0);
	}
}

int main(int argc, char** argv)
{
	generate_polygon_points();
	generate_ET();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(25, 25);
	glutInitWindowSize(880, 880);
	glutCreateWindow(argv[0]);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard_callback);
	glutMainLoop();
	return 0;
}