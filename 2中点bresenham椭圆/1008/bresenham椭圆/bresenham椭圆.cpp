//3.5������չ��ʵ��.cpp
//����ο� https://blog.csdn.net/orbit/article/details/7496008
#define GLUT_DISABLE_ATEXIT_HACK
#include "GLUT.H"
#include <math.h>
#define ZVALUE 20.0f
int w_width = 600;
int w_height = 600;
//��������ϵ
void draw_coordinate()
{
	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0); //���ý�������ʾ����ɫ
	glVertex2f(w_width, 0.0);
	glVertex2f(-w_width, 0.0);
	glColor3f(0.0, 1.0, 0.0);
	glVertex2f(0.0, w_height);
	glVertex2f(0.0, -w_height);
	glEnd();
}
//���öԳƻ��Ƶ�
void draw(int x, int y)
{
	glVertex2d(x, y);
	glVertex2d(x, -y);
	glVertex2d(-x, y);
	glVertex2d(-x, -y);
}
//��bresenham�㷨������Բ ab�ֱ�Ϊ������Ͷ̰��᳤ Ĭ��������ԭ��
void BresenhamEllipse(int a, int b)
{
	int sqa = a * a;
	int sqb = b * b;
	//x��y��ʼ����
	int x = 0;
	int y = b;
	int d = 2 * sqb - 2 * b * sqa + sqa;//��ʼ���ж�ֵ
	draw(x, y);//����
	int P_x = (sqa / sqrt(static_cast<unsigned _int64>(sqa) + sqb));
	while (x <= P_x)
	{
		if (d < 0)
		{
			d += 2 * sqb * (2 * x + 3);
			x++;
		}
		else
		{
			d += 2 * sqb * (2 * x + 3) - 4 * sqa * (y - 1);
			y--;
			x++;
		}
		draw(x, y);
	}
	x = a;
	y = 0;
	draw(x, y);
	d = 2 * sqa - 2 * a * sqb + sqb;
	while (x > P_x)
	{
		if (d < 0)
		{
			d += 2 * sqa * (2 * y + 3);
			y++;
		}
		else
		{
			d += 2 * sqa * (2 * y + 3) - 4 * sqb * (x - 1);
			x--;
			y++;
		}
		draw(x, y);
	}
	/*
	d = sqb * (x * x + x) + sqa * (y * y - y) - sqa * sqb;
	while (y >= 0)
	{
		if (d < 0)
		{
			d -= 2 * sqa * y + sqa - 2 * sqb * x - 2 * sqb;
			y--;
			x++;
		}
		else
		{
			d -= 2 * sqa * y + sqa;
			y--;
		}
		draw(x, y);
	}
	*/
}
//��������
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);
	draw_coordinate(); //��������ϵ
	glColor3f(0, 0, 1);
	glPushMatrix();
	glPointSize(3);//�Ӵ�
	glBegin(GL_POINTS);
	BresenhamEllipse(200, 100);
	glEnd();
	glPopMatrix();
	glFlush();
}
//ͶӰ��ʽ��modelview��ʽ������
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w_width, (GLsizei)w_height);//�ӿڴ�С
	glMatrixMode(GL_PROJECTION);//����ͶӰģʽ�Լ��Ӿ����С
	glLoadIdentity();
	if (w <= h)
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
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Bresenham�㷨������Բ");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}