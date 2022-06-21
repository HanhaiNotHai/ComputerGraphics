#define GLUT_DISABLE_ATEXIT_HACK
#include<list>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "My_3DVector.h"
#include "GLUT.H"
using namespace std;

int nearplane_width = 600; //�Ӿ�����
int nearplane_height = 600; //�Ӿ���߶�
int nearplane_distance = 50; //�Ӿ����ƽ�����ӵ����
int farplane_distance = nearplane_distance + 300; //�Ӿ���Զƽ�����ӵ����

typedef list<my_homogeneous_point_EX> LISTINT;
//��list���������������� 
//��LISTINT����һ����ΪListA,ListB��list����
LISTINT ListA, ListB;
//����iterΪ������
LISTINT::iterator iter;

#define clip_polygon_point_count 4 //�ü�����ζ�����
#define subject_polygon_point_count 4 //���ü�����ζ�����
struct my_homogeneous_point_EX subject_polygon[subject_polygon_point_count]; //���ü������
struct my_homogeneous_point_EX clip_polygon[clip_polygon_point_count]; //�ü������

my_homogeneous_point_EX clip_cur_point;//�ü��ߵ����
my_homogeneous_point_EX clip_next_point;//�ü��ߵ��յ�
my_homogeneous_point_EX subject_cur_point;//���ñߵ����
my_homogeneous_point_EX subject_next_point;//���ü��ߵ��յ�
my_homogeneous_point_EX clip_cross_subject;//�ü��ߺͱ��ü��ߵĽ���

//��ʼ���ü�����Ρ����ü�����ζ������� �����ü�����εĶ��㸳ֵ��ListB��
void init(void)
{
	//���ü�����εĶ������� {0,40},{0,0},{20,20},{65,20},{60,45},{30,45},{5,35}
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

	//�ü�����εĶ������� {-10,50},{10,30},{-5,10},{40,15},{80,35},{30,80}
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

	//��ListB��ֵ
	for (int k = 0; k < subject_polygon_point_count; k++) {
		ListB.push_back(subject_polygon[k]);
	}
}

//��������ϵ
void draw_coordinate()
{
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); //��ɫx��
	glVertex3f(nearplane_width, 0.0, 0.0);
	glVertex3f(-nearplane_width, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0);//��ɫy��
	glVertex3f(0.0, nearplane_height, 0.0);
	glVertex3f(0.0, -nearplane_height, 0.0);
	glEnd();
}

/***************************************
* �������󽻵㺯��
* a1��a2Ϊһ�����ϵ������˵� b1��b2Ϊ��һ�����ϵ������˵�
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
* �жϸ������ڸ����ü��ߵĿɼ��໹�ǲ��ɼ���
* edge_start_point��edge_end_pointΪ��ǰ�ü��ߵ������յ㣻given_pointΪ������
* clip_polygonΪ�ü������
******************************************/
bool determine_point_edge_position(my_homogeneous_point_EX edge_start_point, my_homogeneous_point_EX edge_end_point, my_homogeneous_point_EX given_point)
{
	//����ü�ƽ�������ڵ���������������ͶӰ��ķ�����
	my_3Dvector a(clip_polygon[1], clip_polygon[2]);
	my_3Dvector b(clip_polygon[1], clip_polygon[3]);
	my_3Dvector face_normal = a.cross_multiply(b);

	//�����������������γɽǶ�������ʱ��(+)����˳ʱ��(-)������ü�ƽ�淨������ˣ�������0����ͬΪ��ʱ��ȡ��������ȡ��
	my_3Dvector c(edge_start_point, edge_end_point);//cΪ�ü��߶�����
	my_3Dvector d(edge_start_point, given_point);//dΪ�жϵ���ü��߶�
	my_3Dvector cross_vector = c.cross_multiply(d);

	float sign = cross_vector.dot_multiply(face_normal);
	return sign >= 1e-6;//�ڿɼ��෵��1���ڲ��ɼ��෵��0��
}

/***************************************
* �ü�����Sutherland_Hodgman_Clipping
* clip_polygon��subject_polygon�ֱ�Ϊ�ü�����κͱ��ü������
* clip_polygon_point_count��ʾ�ü�����ζ������
* subject_polygon_point_count��ʾ���ü�����ζ������
******************************************/
void Sutherland_Hodgman_Clipping()
{
	for (int i = 0; i < clip_polygon_point_count; i++)
	{
		//��һ��:ȡ����ǰ��clip_cur_point����һ��clip_next-point
		ListA.clear(); // ListAΪ�������ڴ洢ÿ���ߵĲü����
		int start_vec_index = i % clip_polygon_point_count;
		int end_vec_index = (i + 1) % clip_polygon_point_count;
		clip_cur_point = clip_polygon[start_vec_index];
		clip_next_point = clip_polygon[end_vec_index];
		bool flag = true;

		//�ڶ���:����subject_polygon��ȡ����ǰ��subject_cur_point����һ��subject_next_point,
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

				//���ݹ��򣬽��б��󽻻򶥵�ȡ�ᣬ���е���Ҫ��������뵽ListA��
				if (a == false && b == true)
				{
					clip_cross_subject = GetCrossPoint(clip_cur_point, clip_next_point, subject_cur_point, subject_next_point);
					ListA.push_back(clip_cross_subject);
					ListA.push_back(subject_next_point);
				}
				else if (a == true && b == true)
				{
					//���ڿɼ��� �����յ�
					ListA.push_back(subject_next_point);
				}
				else if (a == true && b == false)
				{
					//����ڿɼ��࣬�յ��ڲ��ɼ��� ���潻��
					clip_cross_subject = GetCrossPoint(clip_cur_point, clip_next_point, subject_cur_point, subject_next_point);
					ListA.push_back(clip_cross_subject);
				}
			}
		ListB.clear();
		//����������ListA���Ƶ�ListB���飬�ٻص���һ��
		//ListB�洢���յĲü����
		for (iter = ListA.begin(); iter != ListA.end(); iter++)
		{
			ListB.push_back(*iter);
		}
	}
}

//���̽����¼�
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

//ͶӰ��ʽ��modelview��ʽ����
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height *
			(GLfloat)nearplane_height / (GLfloat)nearplane_width, 0.5 * nearplane_height *
			(GLfloat)nearplane_height / (GLfloat)nearplane_width,
			-nearplane_distance, farplane_distance); //������ӵ�
	else
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height *
			(GLfloat)nearplane_width / (GLfloat)nearplane_height, 0.5 * nearplane_height *
			(GLfloat)nearplane_width / (GLfloat)nearplane_height,
			-nearplane_distance, farplane_distance);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//��������
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLineWidth(3);//�����ߵĿ��Ϊ3
	draw_coordinate(); //��������ϵ

	glColor3f(0, 0, 0);
	//����clipping_polygon,Ĭ�϶���֮��ͨ��ֱ�߶����� ���Ʋü������
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

	glColor3f(0, 0, 1);//���Ʊ��ü������
	glBegin(GL_LINE_LOOP);
	for (iter = ListB.begin(); iter != ListB.end(); iter++)
	{
		glVertex3i((floor)((*iter).x + 0.5), (floor)((*iter).y + 0.5), 0);
	}
	glEnd();
	glutSwapBuffers();
}

//��������
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