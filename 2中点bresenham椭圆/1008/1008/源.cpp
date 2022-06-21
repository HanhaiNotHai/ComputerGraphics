//3.3 ���� Bresenham �㷨����ֱ�߶�.cpp
#define GLUT_DISABLE_ATEXIT_HACK
#include "GLUT.H"
#include<math.h>
#define ZVALUE 20.0f
float xmove = 0;
float ymove = 0;
int w_width = 500;//�ӿڡ����ڵĿ��
int w_height = 500;//�ӿڡ����ڵĸ߶�
//��������ϵ
void draw_coordinate()
{
	glBegin(GL_LINES);//����ͼԪ�ĺ������������ֱ�ߵ���˼
	glColor3f(1.0, 0.0, 0.0); //���ý�������ʾ����ɫ
	glVertex2f(w_width, 0.0);
	glVertex2f(-w_height, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(0.0, w_width);
	glVertex2f(0.0, -w_height);
	glEnd();
}
//��Breseham����ֱ�� (x0,y0)(x1,y1)Ϊֱ�����˵�
/*
void Bresenham(int x0, int y0, int x1, int y1)
{
	int dy = y1 - y0;
	int dx = x1 - x0;
	int xf = x0;
	int yf = y0;
	int e = -dx;
	while (xf < x1)
	{
		glVertex2d(xf, yf);
		e = e + 2 * dy;
		if (e > 0)
		{
			yf++;
			xf++;
			e = e - 2 * dx;
		}
		else
		{
			xf++;
		}
	}
}
*/
//��������
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);
	draw_coordinate(); //��������ϵ
	/*
	glPointSize(5);
	glColor3f(0, 0, 1);
	glBegin(GL_POINTS);
	glVertex2d(-200, -200);
	glVertex2d(200, 200);
	Bresenham(-200, -200, 200, 200);
	glEnd();
	*/

	glTranslated(40, -100, 0);
	int a = 200, b = 120;
	float xk1 = a * a / sqrt(a * a + b * b);
	float x = 1, y = b - 0.5;
	glColor3f(0, 0, 0);
	//|k|<1
	glBegin(GL_POINTS);
	glVertex2d(0, b);
	glVertex2d(0, -b);
	//glEnd();
	while (x <= xk1)
	{
		if (x * x / a / a + y * y / b / b > 1)
		{
			//glBegin(GL_POINTS);
			glVertex2d(x, y - 0.5);
			glVertex2d(x, 0.5 - y);
			glVertex2d(-x, y - 0.5);
			glVertex2d(-x, 0.5 - y);
			//glEnd();
			y--;
		}
		else
		{
			//glBegin(GL_POINTS);
			glVertex2d(x, y + 0.5);
			glVertex2d(x, -y - 0.5);
			glVertex2d(-x, y + 0.5);
			glVertex2d(-x, -y - 0.5);
			//glEnd();
		}
		x++;
	}
	//|k|>1
	//glBegin(GL_POINTS);
	glVertex2d(a, 0);
	glVertex2d(-a, 0);
	//glEnd();
	y = 1;
	x = a - 0.5;
	while (x > xk1)
	{
		if (x * x / a / a + y * y / b / b > 1)
		{
			//glBegin(GL_POINTS);
			glVertex2d(x - 0.5, y);
			glVertex2d(x - 0.5, -y);
			glVertex2d(0.5 - x, y);
			glVertex2d(0.5 - x, -y);
			//glEnd();
			x--;
		}
		else
		{
			//glBegin(GL_POINTS);
			glVertex2d(x + 0.5, y);
			glVertex2d(x + 0.5, -y);
			glVertex2d(-x - 0.5, y);
			glVertex2d(-x - 0.5, -y);
			//glEnd();
		}
		y++;
	}
	glEnd();
	glFlush();
}
//ͶӰ��ʽ��modelview��ʽ������
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w_width, (GLsizei)w_height);//�ӿڴ�С
	glMatrixMode(GL_PROJECTION);//����ͶӰģʽ�Լ��Ӿ����С
	glLoadIdentity();
	if (w_width <= w_height)
		glOrtho(-0.5 * w_width, 0.5 * w_width, -0.5 * w_height * (GLfloat)w_height /
			(GLfloat)w_width, 0.5 * w_height * (GLfloat)w_height / (GLfloat)w_width,
			-ZVALUE, ZVALUE);
	else
		glOrtho(-0.5 * w_width, 0.5 * w_width, -0.5 * w_height * (GLfloat)w_width /
			(GLfloat)w_height, 0.5 * w_height * (GLfloat)w_width / (GLfloat)w_height,
			-ZVALUE, ZVALUE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
//��������
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w_width, w_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("�е㻭��Բ//Bresenham��ֱ��");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}