//8.2z��������.cpp
#define GLUT_DISABLE_ATEXIT_HACK
#include "GLUT.H"
#include <math.h>
#include <string.h>

int nearplane_width = 600; //�Ӿ�����
int nearplane_height = 600; //�Ӿ���߶�
int nearplane_distance = 500; //�Ӿ����ƽ�����ӵ����
int farplane_distance = nearplane_distance + 300; //�Ӿ���Զƽ�����ӵ����

float eye_x = 20;
float eye_z = 20;
float theta = 0.1;

struct my_v_homogeneous
{
	float x;
	float y;
	float z;
	float ratio;
};

//box��������
//ÿ���߶���ֱ��
struct my_v_homogeneous box[8];

//��ʼ�������ζ�������
void init(void)
{
	//ǰ���ĸ�����
	box[0].x = 0;
	box[0].y = 0;
	box[0].z = 0;
	box[0].ratio = 1;

	box[1].x = 80;
	box[1].y = 0;
	box[1].z = 0;
	box[1].ratio = 1;

	box[2].x = 80;
	box[2].y = 40;
	box[2].z = 0;
	box[2].ratio = 1;

	box[3].x = 0;
	box[3].y = 40;
	box[3].z = 0;
	box[3].ratio = 1;

	//�����ĸ�����
	box[4].x = 0;
	box[4].y = 0;
	box[4].z = -50;
	box[4].ratio = 1;

	box[5].x = 80;
	box[5].y = 0;
	box[5].z = -50;
	box[5].ratio = 1;

	box[6].x = 80;
	box[6].y = 40;
	box[6].z = -50;
	box[6].ratio = 1;

	box[7].x = 0;
	box[7].y = 40;
	box[7].z = -50;
	box[7].ratio = 1;
}

//��������ϵ
void draw_coordinate()
{
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0); //��ɫx��
	glVertex3f(nearplane_width, 0.0, 0.0);
	glVertex3f(-nearplane_width, 0.0, 0.0);

	glColor3f(0.0, 1.0, 0.0); //��ɫy��
	glVertex3f(0.0, nearplane_height, 0.0);
	glVertex3f(0.0, -nearplane_height, 0.0);

	glColor3f(0.0, 0.0, 1.0); //��ɫz��
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
	glColor3f(0.0, 0.0, 0.0);
	glutSolidSphere(30, 50, 50); //��������

	glColor3f(157.0 / 256, 195.0 / 256, 230.0 / 256);

	//����BOX,Ĭ�϶���֮��ͨ��ֱ�߶�����
	glBegin(GL_QUADS);

	//ǰ��
	glVertex3i((floor)(box[0].x + 0.5), (floor)(box[0].y + 0.5), (floor)(box[0].z + 0.5));
	glVertex3i((floor)(box[1].x + 0.5), (floor)(box[1].y + 0.5), (floor)(box[1].z + 0.5));
	glVertex3i((floor)(box[2].x + 0.5), (floor)(box[2].y + 0.5), (floor)(box[2].z + 0.5));
	glVertex3i((floor)(box[3].x + 0.5), (floor)(box[3].y + 0.5), (floor)(box[3].z + 0.5));

	//����
	glVertex3i((floor)(box[4].x + 0.5), (floor)(box[4].y + 0.5), (floor)(box[4].z + 0.5));
	glVertex3i((floor)(box[5].x + 0.5), (floor)(box[5].y + 0.5), (floor)(box[5].z + 0.5));
	glVertex3i((floor)(box[6].x + 0.5), (floor)(box[6].y + 0.5), (floor)(box[6].z + 0.5));
	glVertex3i((floor)(box[7].x + 0.5), (floor)(box[7].y + 0.5), (floor)(box[7].z + 0.5));

	//����
	glVertex3i((floor)(box[4].x + 0.5), (floor)(box[4].y + 0.5), (floor)(box[4].z + 0.5));
	glVertex3i((floor)(box[0].x + 0.5), (floor)(box[0].y + 0.5), (floor)(box[0].z + 0.5));
	glVertex3i((floor)(box[3].x + 0.5), (floor)(box[3].y + 0.5), (floor)(box[3].z + 0.5));
	glVertex3i((floor)(box[7].x + 0.5), (floor)(box[7].y + 0.5), (floor)(box[7].z + 0.5));

	//����
	glVertex3i((floor)(box[1].x + 0.5), (floor)(box[1].y + 0.5), (floor)(box[1].z + 0.5));
	glVertex3i((floor)(box[5].x + 0.5), (floor)(box[5].y + 0.5), (floor)(box[5].z + 0.5));
	glVertex3i((floor)(box[6].x + 0.5), (floor)(box[6].y + 0.5), (floor)(box[6].z + 0.5));
	glVertex3i((floor)(box[2].x + 0.5), (floor)(box[2].y + 0.5), (floor)(box[2].z + 0.5));

	//����
	glVertex3i((floor)(box[3].x + 0.5), (floor)(box[3].y + 0.5), (floor)(box[3].z + 0.5));
	glVertex3i((floor)(box[2].x + 0.5), (floor)(box[2].y + 0.5), (floor)(box[2].z + 0.5));
	glVertex3i((floor)(box[6].x + 0.5), (floor)(box[6].y + 0.5), (floor)(box[6].z + 0.5));
	glVertex3i((floor)(box[7].x + 0.5), (floor)(box[7].y + 0.5), (floor)(box[7].z + 0.5));

	//����
	glVertex3i((floor)(box[0].x + 0.5), (floor)(box[0].y + 0.5), (floor)(box[0].z + 0.5));
	glVertex3i((floor)(box[1].x + 0.5), (floor)(box[1].y + 0.5), (floor)(box[1].z + 0.5));
	glVertex3i((floor)(box[5].x + 0.5), (floor)(box[5].y + 0.5), (floor)(box[5].z + 0.5));
	glVertex3i((floor)(box[4].x + 0.5), (floor)(box[4].y + 0.5), (floor)(box[4].z + 0.5));

	glEnd();
	glutSwapBuffers();
}

//���̽����¼�
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'z': //��Zbuffer��Ȳ���
	case 'Z':
	{
		glEnable(GL_DEPTH_TEST); //����Ȼ������
		glDepthFunc(GL_LEQUAL); //�ж��ڵ���ϵʱ�����ӵ���������ڵ����ӵ�Զ������
		glutPostRedisplay();
		break;
	}
	case 'c': //�ر�Zbuffer��Ȳ���
	case 'C':
	{
		glDisable(GL_DEPTH_TEST); //�ر���Ȼ������
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

	gluLookAt(eye_x, 10, eye_z, 0, 0, 0, 0, 1, 0);
}

//��꽻���¼�
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			eye_x = eye_x * cosf(-theta) + eye_z * sinf(-theta);
			eye_z = eye_z * cosf(-theta) - eye_x * sinf(-theta);
			reshape(nearplane_width, nearplane_height);
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			eye_x = eye_x * cosf(theta) + eye_z * sinf(theta);
			eye_z = eye_z * cosf(theta) - eye_x * sinf(theta);
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
	glutCreateWindow("z����");

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}