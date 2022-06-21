#define GLUT_DISABLE_ATEXIT_HACK
#include<list>
#include "GLUT.H"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "My_3DVector.h"
using namespace std;

int nearplane_width = 600; //�Ӿ�����
int nearplane_height = 600;//�Ӿ���߶�
int nearplane_distance = 50; //�Ӿ����ƽ�����ӵ����
int farplane_distance = nearplane_distance + 300; //�Ӿ���Զƽ�����ӵ����

typedef list<my_homogeneous_point> LISTINT;
//��list���������������� 
//��LISTINT����һ����ΪList��list����
LISTINT List;
//����iterΪ������
LISTINT::iterator iter;

//���Զ��� ֱ�߶˵�
my_homogeneous_point test_point1;
my_homogeneous_point test_point2;

//�ü�����ζ�������
//ÿ���߶���ֱ��
#define clip_polygon_point_count 4
struct my_homogeneous_point clip_polygon[clip_polygon_point_count]; //�ü���

//���ü�����ζ�������
//ÿ���߶���ֱ��
struct my_homogeneous_point subject_polygon[4];

//��ʼ������ֱ���Լ��ü��������
void init(void)
{
	//���Ե�λ�ó�ʼ��
	test_point1.x = 20 * 2;
	test_point1.y = 10 * 2;
	test_point1.z = 0;
	//���Ե�λ�ó�ʼ��
	test_point2.x = 80 * 2;
	test_point2.y = 50 * 2;
	test_point2.z = 0;

	//�ü��������
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

//�����������
struct my_homogeneous_point matrix_multiply_vector(float matrix[][4], struct my_homogeneous_point input_v)
{
	struct my_homogeneous_point translated_v;
	translated_v.x = matrix[0][0] * input_v.x + matrix[0][1] * input_v.y + matrix[0][2] * input_v.z +matrix[0][3] * input_v.ratio;
	translated_v.y = matrix[1][0] * input_v.x + matrix[1][1] * input_v.y + matrix[1][2] * input_v.z +matrix[1][3] * input_v.ratio;
	translated_v.z = matrix[2][0] * input_v.x + matrix[2][1] * input_v.y + matrix[2][2] * input_v.z +matrix[2][3] * input_v.ratio;
	translated_v.ratio = matrix[3][0] * input_v.x + matrix[3][1] * input_v.y + matrix[3][2] *input_v.z + matrix[3][3] * input_v.ratio;
	return translated_v;
}

//���ƽ�Ʊ任
void my_traslate_homogeneous(struct my_homogeneous_point* polygon, int point_count, float tx,float ty, float tz)
{
	//װ������ƽ�ƾ���
	float translate_matrix[4][4];
	memset(translate_matrix, 0, sizeof(int) * 16);
	translate_matrix[0][0] = translate_matrix[1][1] = translate_matrix[2][2] = translate_matrix[3][3]= 1;//�Խ��߸�ֵΪ1
	translate_matrix[0][3] = tx;
	translate_matrix[1][3] = ty;
	translate_matrix[2][3] = tz;

	//������ƽ�ƶ����ÿ������
	for (int vIndex = 0; vIndex < point_count; vIndex++)
	{
		struct my_homogeneous_point input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.z = polygon[vIndex].z;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v); //ƽ�ƾ������õ�ÿ������
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
		polygon[vIndex].z = input_v.z;
		polygon[vIndex].ratio = input_v.ratio;
	}
}

/***************************************
* ���뺯������������꣬���4λ����
* px��pyΪ����� clip_polygonΪ�ü������ clip_polygon_point_countΪ�ü�����εĶ�����
��
* 4λ������������£�������)��
* ��1λ������˵��ڲü�����ε���࣬��Ϊ1������Ϊ0
* ��2λ������˵��ڲü�����ε��Ҳ࣬��Ϊ1������Ϊ0
* ��3λ������˵��ڲü�����ε��²࣬��Ϊ1������Ϊ0
* ��4λ������˵��ڲü�����ε��ϲ࣬��Ϊ1������Ϊ0
���ҵ���
******************************************/
unsigned int enCode(double px, double py)
{
	//��1 2 4 8 ����ü�������ҡ��¡�������
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

//Cohen-Sutherland �ü�����
//�����߶ε������˵� point1 �� point2 �ı����жϣ����߶���ü�����ε�λ�ã�����ü���������˵�
void cilpping_lines(my_homogeneous_point point1, my_homogeneous_point point2)
{
	my_homogeneous_point test_point11 = point1;//��test_point11��test_point22����ֱ�������˵�
	my_homogeneous_point test_point22 = point2;

	float x = 0; //����仯���x��y����
	float y = 0;

	int code1 = enCode(test_point11.x, test_point11.y); //code1��code2��¼����ı��� 
	int code2 = enCode(test_point22.x, test_point22.y);
	int code = 0;

	while (code1 != 0 || code2 != 0) //��ֱ��������һ����������
	{
		if ((code1 & code2) != 0)
		{ //�������߲����ü�����,����ѭ��
			return;
		}

		//����������ĵ�����code
		code = code1;
		if (code1 == 0) code = code2;

		if ((1 & code) != 0) //ֱ�߶�����߽��ཻ�����ֱ����߽��ཻ��
		{
			x = clip_polygon[0].x;
			y = test_point11.y + (test_point22.y - test_point11.y) * (clip_polygon[0].x -test_point11.x) / (test_point22.x - test_point11.x);
		}
		else if ((2 & code) != 0) //ֱ�߶����ұ߽��ཻ�����ֱ����߽��ཻ��
		{
			x = clip_polygon[1].x;
			y = test_point11.y + (test_point22.y - test_point11.y) * (clip_polygon[1].x -test_point11.x) / (test_point22.x - test_point11.x);
		}
		else if ((4 & code) != 0) //ֱ�߶����±߽��ཻ�����ֱ����߽��ཻ��
		{
			y = clip_polygon[0].y;
			x = test_point11.x + (test_point22.x - test_point11.x) * (clip_polygon[0].y -test_point11.y) / (test_point22.y - test_point11.y);
		}
		else if ((8 & code) != 0) //ֱ�߶����ϱ߽��ཻ�����ֱ����߽��ཻ��
		{
			y = clip_polygon[3].y;
			x = test_point11.x + (test_point22.x - test_point11.x) * (clip_polygon[3].y -test_point11.y) / (test_point22.y - test_point11.y);
		}

		//������ĵ����¸�ֵ
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
	//�������ɵĵ����List��
	List.push_back(test_point11);
	List.push_back(test_point22);
}

//���̽����¼�
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	case 'W':
	{
		my_traslate_homogeneous(clip_polygon, clip_polygon_point_count, 0, 1, 0);//��Y���������ƶ�1����Ԫ
		glutPostRedisplay();
		break;
	}
	case 's':
	case 'S':
	{
		my_traslate_homogeneous(clip_polygon, clip_polygon_point_count, 0, -1, 0);//��Y�Ḻ�����ƶ�1����Ԫ
		glutPostRedisplay();
		break;
	}
	case 'a':
	case 'A':
	{
		my_traslate_homogeneous(clip_polygon, clip_polygon_point_count, -1, 0, 0);//��X�Ḻ�����ƶ�1����Ԫ
		glutPostRedisplay();
		break;
	}
	case 'd':
	case 'D':
	{
		my_traslate_homogeneous(clip_polygon, clip_polygon_point_count, 1, 0, 0); //��X���������ƶ�1����Ԫ
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
	glLineWidth(3);//����ֱ�߿��
	draw_coordinate(); //��������ϵ

	glColor3f(157.0 / 256, 195.0 / 256, 230.0 / 256);
	//����clipping_polygon,Ĭ�϶���֮��ͨ��ֱ�߶����� ���Ʋü�����

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

	glBegin(GL_LINES);//���Ʊ��ü�ֱ��
	for (std::list<my_homogeneous_point>::iterator iter = List.begin(); iter != List.end(); iter++)
	{
		glVertex3i((floor)(iter->x + 0.5), (floor)(iter->y + 0.5), (floor)(iter->z + 0.5));
	}
	glEnd();

	List.clear();
	glutSwapBuffers();
}

//��������
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(nearplane_width, nearplane_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Cohen-Sutherlandֱ�߶βü�");

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();
	return 0;
}