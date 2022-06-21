#define GLUT_DISABLE_ATEXIT_HACK
#include <vector>
#include "GLUT.H" 
#include "freeform_head.h"
 
using namespace std;

int nearplane_width = 600; //�Ӿ�����
int nearplane_height = 600; //�Ӿ���߶�
int nearplane_distance = 500; //�Ӿ����ƽ�����ӵ����
int farplane_distance = nearplane_distance + 300; //�Ӿ���Զƽ�����ӵ����

float eye_x = 20;
float eye_y = 20;
float eye_z = 20;
float theta = 0.1;
float radius = 20;
 
//���ڴ���������õ���ά�ռ��еĵ�
vector<my_3D_point_coord> view_3D_points;
my_3D_point_coord ctrl_points[4][4];
bool drawing = false; //�Ƿ�ʼ����

void init()
{
	my_3D_point_coord p;

	p.x = 10;
	p.y = 10;
	p.z = 0;
	view_3D_points.push_back(p);

	p.x = 10;
	p.y = 110;
	p.z = 0;
	view_3D_points.push_back(p);

	p.x = 110;
	p.y = 110;
	p.z = 0;
	view_3D_points.push_back(p);

	p.x = 110;
	p.y = 10;
	p.z = 0;
	view_3D_points.push_back(p);

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	ctrl_points[0][0].x = 200; ctrl_points[0][0].y = 20; ctrl_points[0][0].z = 0;//p00
	ctrl_points[0][1].x = 150; ctrl_points[0][1].y = 0; ctrl_points[0][1].z = 100;//p01
	ctrl_points[0][2].x = 50; ctrl_points[0][2].y = -130; ctrl_points[0][2].z = 100;//p02
	ctrl_points[0][3].x = 0; ctrl_points[0][3].y = -250; ctrl_points[0][3].z = 50;//p03


	ctrl_points[1][0].x = 150; ctrl_points[1][0].y = 100; ctrl_points[1][0].z = 100;//p10
	ctrl_points[1][1].x = 100; ctrl_points[1][1].y = 30; ctrl_points[1][1].z = 100;//p11
	ctrl_points[1][2].x = 50; ctrl_points[1][2].y = -40; ctrl_points[1][2].z = 100;//p12
	ctrl_points[1][3].x = 0; ctrl_points[1][3].y = -110; ctrl_points[1][3].z = 100;//p13


	ctrl_points[2][0].x = 140; ctrl_points[2][0].y = 280; ctrl_points[2][0].z = 90;//p20
	ctrl_points[2][1].x = 80; ctrl_points[2][1].y = 110; ctrl_points[2][1].z = 120;//p21
	ctrl_points[2][2].x = 30; ctrl_points[2][2].y = 30; ctrl_points[2][2].z = 130;//p22
	ctrl_points[2][3].x = -50; ctrl_points[2][3].y = -100; ctrl_points[2][3].z = 150;//p23

	ctrl_points[3][0].x = 150; ctrl_points[3][0].y = 350; ctrl_points[3][0].z = 30;//p30
	ctrl_points[3][1].x = 50; ctrl_points[3][1].y = 200; ctrl_points[3][1].z = 150;//p31
	ctrl_points[3][2].x = 0; ctrl_points[3][2].y = 50; ctrl_points[3][2].z = 200;//p32
	ctrl_points[3][3].x = -70; ctrl_points[3][3].y = 0; ctrl_points[3][3].z = 100;//p33
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 

	draw_coordinate(); 
	 
	//���ƿ��ƶ����
	glColor3f(0, 0, 0);
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	/*int max_point_index = view_3D_points.size() - 1;
	glBegin(GL_POINTS);
	    glPointSize(15);
		for (int pindex = 0; pindex <= max_point_index; pindex++)
		{
			glVertex3f(view_3D_points[pindex].x, view_3D_points[pindex].y, view_3D_points[pindex].z);
		}
	glEnd();

	glBegin(GL_LINES);		
		for (int pindex = 0; pindex < max_point_index; pindex++)
		{
			glVertex3f(view_3D_points[pindex].x, view_3D_points[pindex].y, view_3D_points[pindex].z);
			glVertex3f(view_3D_points[pindex+1].x, view_3D_points[pindex+1].y, view_3D_points[pindex+1].z);
		}	 
	glEnd();*/
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			glVertex3f(ctrl_points[i][j].x, ctrl_points[i][j].y, ctrl_points[i][j].z);
	glEnd();

	glBegin(GL_LINES);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 3; j++)
		{
			glVertex3f(ctrl_points[i][j].x, ctrl_points[i][j].y, ctrl_points[i][j].z);
			glVertex3f(ctrl_points[i][j + 1].x, ctrl_points[i][j + 1].y, ctrl_points[i][j + 1].z);
		}
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 4; j++)
		{
			glVertex3f(ctrl_points[i][j].x, ctrl_points[i][j].y, ctrl_points[i][j].z);
			glVertex3f(ctrl_points[i + 1][j].x, ctrl_points[i + 1][j].y, ctrl_points[i + 1][j].z);
		}
	glEnd();
	///////////////////////////////////////////////////////////////////////////////////////////////////////

	if (drawing == true)
	{
		glColor3f(1, 0, 0);
		//DrawBezierCurve(view_3D_points);
		//DrawBezierSurface(ctrl_points);

		glColor3f(0, 0, 1);
		//Draw_B3curves(view_3D_points);
		//Draw_B3Surface(ctrl_points);

		glColor3f(0, 1, 0);
		//my_Draw_B3curves(view_3D_points);
		my_Draw_B3Surface(ctrl_points);

		//drawing = false; //ֻ����һ��
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
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width, 0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width,
			-nearplane_distance, farplane_distance); //������ӵ�
	else
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height * (GLfloat)nearplane_width / (GLfloat)nearplane_height, 0.5 * nearplane_height * (GLfloat)nearplane_width / (GLfloat)nearplane_height,
			-nearplane_distance, farplane_distance);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_x, eye_y, eye_z, 0, 0, 0, 0, 1, 0);
}

//���̽����¼�
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case'a':
	{
		eye_x = eye_x * cosf(-theta) + eye_z * sinf(-theta);
		eye_z = eye_z * cosf(-theta) - eye_x * sinf(-theta);
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case'd':
	{
		eye_x = eye_x * cosf(theta) + eye_z * sinf(theta);
		eye_z = eye_z * cosf(theta) - eye_x * sinf(theta);
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case'w':
	{
		eye_y = eye_y * cosf(theta) + eye_z * sinf(theta);
		eye_z = eye_z * cosf(theta) - eye_y * sinf(theta);
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case's':
	{
		eye_y = eye_y * cosf(-theta) + eye_z * sinf(-theta);
		eye_z = eye_z * cosf(-theta) - eye_y * sinf(-theta);
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case'q':
	{
		eye_x = eye_x * cosf(-theta) + eye_y * sinf(-theta);
		eye_y = eye_y * cosf(-theta) - eye_x * sinf(-theta);
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case'e':
	{
		eye_x = eye_x * cosf(theta) + eye_y * sinf(theta);
		eye_y = eye_y * cosf(theta) - eye_x * sinf(theta);
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case '1':
	{
		drawing = true;
		glutPostRedisplay();
		break;
	}
	case '2':
	{
		drawing = false;
		glutPostRedisplay();
		break;
	}
	case 'z'://��Zbuffer��Ȳ���
	case 'Z':
	{
		glEnable(GL_DEPTH_TEST);  // ����Ȼ������
		glDepthFunc(GL_LESS);   // �ж��ڵ���ϵʱ�����ӵ���������ڵ����ӵ�Զ������ 
		glutPostRedisplay();
		break;
	}
	case 'c'://�ر�Zbuffer��Ȳ���
	case 'C':
	{
		glDisable(GL_DEPTH_TEST);  // ����Ȼ������ 
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

//��ȡ�������ӵ�����ϵ�µ�λ��
void Get3DViewpos(int x, int y, my_3D_point_coord* pp) 
{
	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	GLfloat winX, winY, winZ = 0.0;
	GLdouble object_x, object_y, object_z;
	int mouse_x = x;
	int mouse_y = y;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	winX = (float)mouse_x;
	winY = (float)viewport[3] - (float)mouse_y - 1.0f;
	glReadBuffer(GL_BACK);
	glReadPixels(mouse_x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);
	gluUnProject((GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, modelview, projection, viewport, &object_x, &object_y, &object_z);
	pp->x = object_x;
	pp->y = object_y;
	pp->z = object_z;
}
 
//��꽻���¼�
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			/*eye_x = radius * cosf(theta);
			eye_z = radius * sinf(theta);
			theta += 0.1;
			reshape(nearplane_width, nearplane_height);
			glShadeModel(GL_FLAT);
			glutPostRedisplay();*/
			my_3D_point_coord view_pos = { 0.0, 0.0, 0.0 };
			Get3DViewpos(x, y, &view_pos);
			view_3D_points.push_back(view_pos);
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			eye_x = radius * cosf(theta);
			eye_z = radius * sinf(theta);
			theta -= 0.1;
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
	glutCreateWindow("Freeform");

	init();
 
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}