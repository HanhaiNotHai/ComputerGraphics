#define GLUT_DISABLE_ATEXIT_HACK
#include <glut.h>

#define ZVALUE 20.0f
float x1 = 0, y1 = 0, z1 = 0;//tri
float x2 = 0, y2 = 0, z2 = 0;//quad
GLfloat rtri = 0, rquad = 0;

//��������ϵ
void draw_coordinate()
{
	glBegin(GL_LINES);//����ͼԪ�ĺ������������ֱ�ߵ���˼
	glColor3f(1.0, 0.0, 0.0); //���ý�������ʾ����ɫ��R��G��B��
	glVertex2f(15.0, 0.0);//�Ҷ���
	glVertex2f(-15.0, 0.0);//�󶥵�

	glColor3f(0.0, 1.0, 0.0);//���ý�������ʾ����ɫ
	glVertex2f(0.0, 15.0);//�϶���
	glVertex2f(0.0, -15.0);//�¶���
	glEnd();//���ƽ���
}

void display(void)
{
	glClearColor(0.f, 0.f, 0.f, 0.f);//���ô��ڱ���ɫ
	glClear(GL_COLOR_BUFFER_BIT);//�õ�ǰ����ɫ��䴰��
	draw_coordinate(); //��������ϵ���Զ��庯��
	glLoadIdentity();

	glPushMatrix();
	glTranslatef(x1, y1, z1);
	glRotatef(rtri, 0.0f, 1.0f, 0.0f);
	glBegin(GL_TRIANGLES);
	glColor3f(1, 1, 0);
	glVertex2f(-2, 4);
	glColor3f(1, 0, 1);
	glVertex2f(-4, 0);
	glColor3f(0, 1, 1);
	glVertex2f(0, 0);
	glEnd();
	glPopMatrix();

	//�����ı���
	glPushMatrix();
	glTranslatef(x2, y2, z2);
	glRotatef(rquad, 0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS); //GL_LING_LOOP
	glColor3f(0, 1, 1);
	glVertex2f(0, 0);
	glColor3f(1, 0, 1);
	glVertex2f(0, -3);
	glColor3f(1, 1, 0);
	glVertex2f(10, -3);
	glColor3f(0.75, 0.75, 0.75);
	glVertex2f(10, 0);
	glEnd();
	glPopMatrix();

	glFlush();//���OpenGL���������ִ��OpenGL����
}

//���̽����¼�
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	{
		y1 += 1; //�˴����Ե��ú����ı�ͼ�οռ���Ԫ�ص�λ�û���̬��ʵ�ּ��̽���
		glutPostRedisplay();//���»��ƣ����Զ�����display����
		break;
	}
	case 's':
	{
		y1 -= 1; //�˴����Ե��ú����ı�ͼ�οռ���Ԫ�ص�λ�û���̬��ʵ�ּ��̽���
		glutPostRedisplay();//���»��ƣ����Զ�����display����
		break;
	}
	case 'd':
	{
		x1 += 1; //�˴����Ե��ú����ı�ͼ�οռ���Ԫ�ص�λ�û���̬��ʵ�ּ��̽���
		glutPostRedisplay();//���»��ƣ����Զ�����display����
		break;
	}
	case 'a':
	{
		x1 -= 1; //�˴����Ե��ú����ı�ͼ�οռ���Ԫ�ص�λ�û���̬��ʵ�ּ��̽���
		glutPostRedisplay();//���»��ƣ����Զ�����display����
		break;
	}
	case'q':
	{
		rtri -= 10;
		glutPostRedisplay();
		break;
	}
	case'e':
	{
		rtri += 10;
		glutPostRedisplay();
		break;
	}
	case 'i':
	{
		y2 += 1; //�˴����Ե��ú����ı�ͼ�οռ���Ԫ�ص�λ�û���̬��ʵ�ּ��̽���
		glutPostRedisplay();//���»��ƣ����Զ�����display����
		break;
	}
	case 'k':
	{
		y2 -= 1; //�˴����Ե��ú����ı�ͼ�οռ���Ԫ�ص�λ�û���̬��ʵ�ּ��̽���
		glutPostRedisplay();//���»��ƣ����Զ�����display����
		break;
	}
	case 'l':
	{
		x2 += 1; //�˴����Ե��ú����ı�ͼ�οռ���Ԫ�ص�λ�û���̬��ʵ�ּ��̽���
		glutPostRedisplay();//���»��ƣ����Զ�����display����
		break;
	}
	case 'j':
	{
		x2 -= 1; //�˴����Ե��ú����ı�ͼ�οռ���Ԫ�ص�λ�û���̬��ʵ�ּ��̽���
		glutPostRedisplay();//���»��ƣ����Զ�����display����
		break;
	}
	case'u':
	{
		rquad -= 10;
		glutPostRedisplay();
		break;
	}
	case'o':
	{
		rquad += 10;
		glutPostRedisplay();
		break;
	}
	case 27:
		exit(0);
		break;
	}
}

//��꽻���¼�
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)//������
		{
			x1 += 1; //�˴����Ե��ú����ı�ͼ�οռ���Ԫ�ص�λ�û���̬��ʵ����꽻��
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			y1 += 1; //�˴����Ե��ú����ı�ͼ�οռ���Ԫ�ص�λ�û���̬��ʵ����꽻��
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);///�ӿڱ任
	glMatrixMode(GL_PROJECTION);//����ͶӰģʽ�Լ��Ӿ����С
	glLoadIdentity();//����ͶӰ�任����
	if (w <= h)//ƽ��ͶӰ
		glOrtho(-16.0, 16.0, -16.0 * (GLfloat)h / (GLfloat)w,
			16.0 * (GLfloat)h / (GLfloat)w, -ZVALUE, ZVALUE);
	else
		glOrtho(-16.0 * (GLfloat)h / (GLfloat)w,
			16.0 * (GLfloat)h / (GLfloat)w, -16.0, 16.0, -ZVALUE, ZVALUE);
	glMatrixMode(GL_MODELVIEW); //��ʼ����ģ���ӵ�任����
	glLoadIdentity();//����ģ���ӵ�任����
}

//��������
int main(int argc, char** argv)
{
	glutInit(&argc, argv);//��ʼ��GLUT��
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//��ʼ����ʾģʽ
	glutInitWindowSize(500, 500);//���ô��ڵĴ�С
	glutInitWindowPosition(0, 0);//���ô��ڵ�λ��
	glutCreateWindow("100�֣�����wasd ijkl�ƶ� qe uo��ת");//��������

	glutReshapeFunc(reshape);//���ô��ڳߴ�任ʱ��Ӧ�Ļص�����
	glutDisplayFunc(display);//���û���ʱ��Ӧ�Ļص�����
	glutKeyboardFunc(keyboard);//���ü��̽���ʱ��Ӧ�Ļص�����
	glutMouseFunc(mouse);//������꽻��ʱ��Ӧ�Ļص�����
	glutMainLoop();//����ͼ�γ���
	return 0;
}