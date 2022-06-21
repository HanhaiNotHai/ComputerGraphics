//7.2 ɨ���������������.cpp
#include <stdlib.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include "glut.h"
#include <math.h>
#include <vector>
#include <map>
#define epsion 1e-3
struct my_v_inhomogeneous
{
	float x;
	float y;
};
//����α�
struct Edge
{
	float cur_X;//��ǰɨ�����뵱ǰ�߽����x����ֵ
	float reciprocal_K;//��ǰ��б�ʵĵ���
	int max_Y;//��ǰ�����˵�����y����ֵ
	Edge(int y, float x, float rec_k)
	{
		max_Y = y;
		cur_X = x;
		reciprocal_K = rec_k;
	}
};
std::vector<my_v_inhomogeneous> polygon;
std::map<int, std::vector<Edge>> ET;//����yֵ�����б߽��з�����
std::vector<Edge> AET;//��߱�
bool filling = false;
int y_min, y_max;
void generate_polygon_points()
{
	my_v_inhomogeneous p01;
	p01.x = -200, p01.y = -180;
	y_min = p01.y;
	y_max = p01.y;
	polygon.push_back(p01);
	my_v_inhomogeneous p02;
	p02.x = 50, p02.y = -250;
	y_min = p02.y < y_min ? p02.y : y_min;
	y_max = p02.y > y_max ? p02.y : y_max;
	polygon.push_back(p02);
	my_v_inhomogeneous p03;
	p03.x = 200, p03.y = -50;
	y_min = p03.y < y_min ? p03.y : y_min;
	y_max = p03.y > y_max ? p03.y : y_max;
	polygon.push_back(p03);
	my_v_inhomogeneous p04;
	p04.x = 200, p04.y = 220;
	y_min = p04.y < y_min ? p04.y : y_min;
	y_max = p04.y > y_max ? p04.y : y_max;
	polygon.push_back(p04);
	my_v_inhomogeneous p05;
	p05.x = -50, p05.y = 100;
	y_min = p05.y < y_min ? p05.y : y_min;
	y_max = p05.y > y_max ? p05.y : y_max;
	polygon.push_back(p05);
	my_v_inhomogeneous p06;
	p06.x = -200, p06.y = 200;
	y_min = p06.y < y_min ? p06.y : y_min;
	y_max = p06.y > y_max ? p06.y : y_max;
	polygon.push_back(p06);
}
/***************************************
* ���ɶ���ε�ET��
* polygon����ʱ�����еĶ�������
* ����polygon,����������֮�乹��һ����Edge����ͬʱ����������y������Сֵ����
���б߽��з���Ͳ�������
***************************************/
void generate_ET()
{
	for (int eindex = 0; eindex < polygon.size(); eindex++)
	{
		my_v_inhomogeneous startPoint = polygon[eindex];
		my_v_inhomogeneous endPoint = (eindex == (polygon.size() - 1)) ? polygon[0] :
			polygon[eindex + 1];
		if (endPoint.y == startPoint.y) continue;//ˮƽ�߲�����ET��
		//����һ����
		my_v_inhomogeneous top_point = startPoint.y > endPoint.y ? startPoint : endPoint;
		my_v_inhomogeneous bottom_point = startPoint.y < endPoint.y ? startPoint : endPoint;
		float rec_k = (top_point.x - bottom_point.x) / (top_point.y - bottom_point.y);
		Edge one_ET_Edge(top_point.y, bottom_point.x, rec_k);
		//�Ա����˵�y������СֵΪ���ݶԱ߽��з���Ͳ�������
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
			for (std::vector<Edge>::iterator eit = it->second.begin(); eit != it->second.end();
				eit++)
			{
				if (bottom_point.x < eit->cur_X || (bottom_point.x == eit->cur_X && rec_k <
					eit->reciprocal_K))
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
* ��AET�в����µ�Edge���ϣ�������
* polygon����ʱ�����еĶ�������
***************************************/
void insert_new_edges_into_AET(std::vector<Edge> newedges)
{
	//����AET�бߵ�������򣬶�ÿ���߽��в�������
	for (std::vector<Edge>::iterator newHead = newedges.begin(); newHead != newedges.end();
		newHead++)
	{
		bool inserted = false;
		for (std::vector<Edge>::iterator AET_Head = AET.begin(); AET_Head != AET.end();
			AET_Head++)
		{
			if (newHead->cur_X < AET_Head->cur_X || (newHead->cur_X ==
				AET_Head->cur_X && newHead->reciprocal_K < AET_Head->reciprocal_K))
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
* ɨ�������
***************************************/
void scanline_filling()
{
	AET.clear();
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int ypos = y_min; ypos <= y_max; ypos += 1)//���б�������ɨ����
	{
		//�ҵ��������˵�y������СֵΪ�뵱ǰɨ����ypos��ͬ�����б�
		std::map<int, std::vector<Edge>>::iterator it = ET.find(ypos);
		if (it != ET.end())
		{
			insert_new_edges_into_AET(it->second);//���뵽AET����
		}
		std::vector<int> need_removed_edge_index;//�����Ҫɾ���ı����
		 //ȷ������߶��������
		glBegin(GL_LINES);
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
}
void display()
{
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//����ͼ�� 
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	for (int index = 0; index < polygon.size(); index++)
	{
		glVertex2f(polygon[index].x, polygon[index].y);
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
	glViewport(0, 0, w, h); //��������Ӿ����ȡ�ĳ������������ĸߺͿ���ʾ����Ļ��
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
	glutInitWindowPosition(200, 100);
	glutInitWindowSize(500, 500);
	glutCreateWindow(argv[0]);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard_callback);
	glutMainLoop();
	return 0;
}