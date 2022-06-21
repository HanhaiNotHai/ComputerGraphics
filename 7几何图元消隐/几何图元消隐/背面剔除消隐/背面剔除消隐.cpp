//8.3 �����޳�����.cpp
#define GLUT_DISABLE_ATEXIT_HACK
#include <List>
#include <vector>
#include "GLUT.H"
#include <math.h>
#include <string.h>
#include "My_3DVector.h"
using namespace std;

int nearplane_width = 600; //�Ӿ�����
int nearplane_height = 600; //�Ӿ���߶�
int nearplane_distance = 500; //�Ӿ����ƽ�����ӵ����
int farplane_distance = nearplane_distance + 300; //�Ӿ���Զƽ�����ӵ����

float eye_x = 20;
float eye_y = 20;
float eye_z = 20;
float theta = 0.1; //�ӵ�ƫת�Ƕ�

//������Ľṹ�壬��������ĸ�������ͷ�����
struct my_face_homogeneous
{
	list<my_homogeneous_point> mList;//�������㰴����ʱ���˳�򴢴�
	my_3Dvector n; //�����淨����
};

vector<my_face_homogeneous> model; //�����洢��άͼ�ε�10����
my_homogeneous_point facepoint, a, b, c;//�����洢��ʱ��
my_3Dvector N1, N2, N3;//�����洢�����Լ�������ķ�����
my_3Dvector v(eye_x, eye_y, eye_x);//������v�洢�ӵ㷽��
my_face_homogeneous tempt; //���ڴ洢�����Ϣ

//��ʼ����άͼ�ζ�������
void init(void)
{
	//��һ
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

	N1 = my_3Dvector(a, b);//N1�����洢��ǰƽ���ϵ�һ������
	N2 = my_3Dvector(a, c);//N2�����洢��ǰƽ���ϵ�һ������
	N3 = N1.cross_multiply(N2);//N3Ϊ��ǰƽ��ķ�����
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//���
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

	N1 = my_3Dvector(a, b);//N1�����洢��ǰƽ���ϵ�һ������
	N2 = my_3Dvector(a, c);//N2�����洢��ǰƽ���ϵ�һ������
	N3 = N1.cross_multiply(N2);//N3Ϊ��ǰƽ��ķ�����
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//����
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
	N1 = my_3Dvector(a, b);//N1�����洢��ǰƽ���ϵ�һ������
	N2 = my_3Dvector(a, c);//N2�����洢��ǰƽ���ϵ�һ������
	N3 = N1.cross_multiply(N2);//N3Ϊ��ǰƽ��ķ�����
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//����
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

	N1 = my_3Dvector(a, b);//N1�����洢��ǰƽ���ϵ�һ������
	N2 = my_3Dvector(a, c);//N2�����洢��ǰƽ���ϵ�һ������
	N3 = N1.cross_multiply(N2);//N3Ϊ��ǰƽ��ķ�����
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//����
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

	N1 = my_3Dvector(a, b);//N1�����洢��ǰƽ���ϵ�һ������
	N2 = my_3Dvector(a, c);//N2�����洢��ǰƽ���ϵ�һ������
	N3 = N1.cross_multiply(N2);//N3Ϊ��ǰƽ��ķ�����
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//����
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

	N1 = my_3Dvector(a, b);//N1�����洢��ǰƽ���ϵ�һ������
	N2 = my_3Dvector(a, c);//N2�����洢��ǰƽ���ϵ�һ������
	N3 = N1.cross_multiply(N2);//N3Ϊ��ǰƽ��ķ�����
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//����
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

	N1 = my_3Dvector(a, b);//N1�����洢��ǰƽ���ϵ�һ������
	N2 = my_3Dvector(a, c);//N2�����洢��ǰƽ���ϵ�һ������
	N3 = N1.cross_multiply(N2);//N3Ϊ��ǰƽ��ķ�����
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//���
	facepoint.x = 0;
	facepoint.y = 0;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);
	a = facepoint;//a�����洢��ǰƽ���ϵ�һ����

	facepoint.x = 0;
	facepoint.y = 50;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);
	b = facepoint;//b�����洢��ǰƽ���ϵ�һ����

	facepoint.x = 80;
	facepoint.y = 50;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);
	c = facepoint;//c�����洢��ǰƽ���ϵ�һ����

	facepoint.x = 80;
	facepoint.y = 0;
	facepoint.z = 0;
	tempt.mList.push_back(facepoint);

	N1 = my_3Dvector(a, b);//N1�����洢��ǰƽ���ϵ�һ������
	N2 = my_3Dvector(a, c);//N2�����洢��ǰƽ���ϵ�һ������
	N3 = N1.cross_multiply(N2);//N3Ϊ��ǰƽ��ķ�����
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//���
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

	N1 = my_3Dvector(a, b);//N1�����洢��ǰƽ���ϵ�һ������
	N2 = my_3Dvector(a, c);//N2�����洢��ǰƽ���ϵ�һ������
	N3 = N1.cross_multiply(N2);//N3Ϊ��ǰƽ��ķ�����
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();

	//��ʮ
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

	N1 = my_3Dvector(a, b);//N1�����洢��ǰƽ���ϵ�һ������
	N2 = my_3Dvector(a, c);//N2�����洢��ǰƽ���ϵ�һ������
	N3 = N1.cross_multiply(N2);//N3Ϊ��ǰƽ��ķ�����
	tempt.n.dx = N3.dx;
	tempt.n.dy = N3.dy;
	tempt.n.dz = N3.dz;

	model.push_back(tempt);
	tempt.mList.clear();
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

	glColor3f(0.0, 0.0, 1.0);//��ɫz��
	glVertex3f(0.0, 0.0, nearplane_height);
	glVertex3f(0.0, 0.0, -nearplane_height);

	glEnd();
}

//��������
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_coordinate(); //��������ϵ 

	glColor3f(157.0 / 256, 195.0 / 256, 230.0 / 256);

	//�����޳������㷨��ʵ��
	for (int i = 0; i < 10; i++)
	{
		//float num = model[i].n.dot_multiply(v);//ģ�͵�i���淨���������������
		if (model[i].n.dot_multiply(v) > 0)//���ƿɼ���
		{
			//���´�����Ϊ���ò�ͬƽ����ֲ�ͬ��ɫ
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
	glutSwapBuffers();
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

	gluLookAt(eye_x, eye_y, eye_z, 0, 0, 0, 0, 1, 0);
}

//���̽����¼�
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

//��꽻���¼�
//������ı��ӵ㷽��
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

//��������
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(nearplane_width, nearplane_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("�����޳��㷨");

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}