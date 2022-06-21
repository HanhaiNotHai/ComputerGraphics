#define GLUT_DISABLE_ATEXIT_HACK
#include "GLUT.H"

#define ZVALUE 20.0f
float xmove = 0;
float ymove = 0;
//��������ϵ
void draw_coordinate()
{
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0); //���ý�������ʾ����ɫ
		glVertex2f(15.0, 0.0);
		glVertex2f(-15.0, 0.0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex2f(0.0, 15.0);
		glVertex2f(0.0, -15.0);
	glEnd();
}

//��������
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);

	draw_coordinate(); //��������ϵ 
  
	glColor3f(0, 0, 1); // 
	glPushMatrix();
	    glTranslatef(xmove, ymove, 0);
		glBegin(GL_QUADS); //GL_LINE_LOOP
			glVertex2f(0, 0);
			glVertex2f(0, -3);
			glVertex2f(10, -3);
			glVertex2f(10, 0);
		glEnd();
	glPopMatrix();
	glFlush();
}
 
//���̽����¼�
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':
	case 'X':
	{
		xmove += 1;
		glutPostRedisplay();
		break;
	}
	case 'y':
	case 'Y':
	{
		ymove += 1;
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
		if (state == GLUT_DOWN)
		{
			xmove += 1;
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			ymove += 1;
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

//ͶӰ��ʽ��modelview��ʽ������
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);//�ӿڴ�С
	glMatrixMode(GL_PROJECTION);//����ͶӰģʽ�Լ��Ӿ����С
	glLoadIdentity();
	if (w <= h)
		glOrtho(-16.0, 16.0, -16.0*(GLfloat)h / (GLfloat)w, 16.0*(GLfloat)h / (GLfloat)w,
		-ZVALUE, ZVALUE);
	else
		glOrtho(-16.0*(GLfloat)h / (GLfloat)w, 16.0*(GLfloat)h / (GLfloat)w, -16.0, 16.0,
		-ZVALUE, ZVALUE);
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity();
}

//��������
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(900, 900);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("HelloWorld");
 
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}
