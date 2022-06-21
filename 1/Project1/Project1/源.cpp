#define GLUT_DISABLE_ATEXIT_HACK
#include <glut.h>

#define ZVALUE 20.0f
float x1 = 0, y1 = 0, z1 = 0;//tri
float x2 = 0, y2 = 0, z2 = 0;//quad
GLfloat rtri = 0, rquad = 0;

//绘制坐标系
void draw_coordinate()
{
	glBegin(GL_LINES);//绘制图元的函数，代表绘制直线的意思
	glColor3f(1.0, 0.0, 0.0); //设置接下来显示的颜色（R，G，B）
	glVertex2f(15.0, 0.0);//右顶点
	glVertex2f(-15.0, 0.0);//左顶点

	glColor3f(0.0, 1.0, 0.0);//设置接下来显示的颜色
	glVertex2f(0.0, 15.0);//上顶点
	glVertex2f(0.0, -15.0);//下顶点
	glEnd();//绘制结束
}

void display(void)
{
	glClearColor(0.f, 0.f, 0.f, 0.f);//设置窗口背景色
	glClear(GL_COLOR_BUFFER_BIT);//用当前背景色填充窗口
	draw_coordinate(); //绘制坐标系，自定义函数
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

	//绘制四边形
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

	glFlush();//清空OpenGL命令缓冲区，执行OpenGL命令
}

//键盘交互事件
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	{
		y1 += 1; //此处可以调用函数改变图形空间中元素的位置或姿态，实现键盘交互
		glutPostRedisplay();//重新绘制，会自动调用display函数
		break;
	}
	case 's':
	{
		y1 -= 1; //此处可以调用函数改变图形空间中元素的位置或姿态，实现键盘交互
		glutPostRedisplay();//重新绘制，会自动调用display函数
		break;
	}
	case 'd':
	{
		x1 += 1; //此处可以调用函数改变图形空间中元素的位置或姿态，实现键盘交互
		glutPostRedisplay();//重新绘制，会自动调用display函数
		break;
	}
	case 'a':
	{
		x1 -= 1; //此处可以调用函数改变图形空间中元素的位置或姿态，实现键盘交互
		glutPostRedisplay();//重新绘制，会自动调用display函数
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
		y2 += 1; //此处可以调用函数改变图形空间中元素的位置或姿态，实现键盘交互
		glutPostRedisplay();//重新绘制，会自动调用display函数
		break;
	}
	case 'k':
	{
		y2 -= 1; //此处可以调用函数改变图形空间中元素的位置或姿态，实现键盘交互
		glutPostRedisplay();//重新绘制，会自动调用display函数
		break;
	}
	case 'l':
	{
		x2 += 1; //此处可以调用函数改变图形空间中元素的位置或姿态，实现键盘交互
		glutPostRedisplay();//重新绘制，会自动调用display函数
		break;
	}
	case 'j':
	{
		x2 -= 1; //此处可以调用函数改变图形空间中元素的位置或姿态，实现键盘交互
		glutPostRedisplay();//重新绘制，会自动调用display函数
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

//鼠标交互事件
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)//左键点击
		{
			x1 += 1; //此处可以调用函数改变图形空间中元素的位置或姿态，实现鼠标交互
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			y1 += 1; //此处可以调用函数改变图形空间中元素的位置或姿态，实现鼠标交互
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);///视口变换
	glMatrixMode(GL_PROJECTION);//设置投影模式以及视景体大小
	glLoadIdentity();//重置投影变换矩阵
	if (w <= h)//平行投影
		glOrtho(-16.0, 16.0, -16.0 * (GLfloat)h / (GLfloat)w,
			16.0 * (GLfloat)h / (GLfloat)w, -ZVALUE, ZVALUE);
	else
		glOrtho(-16.0 * (GLfloat)h / (GLfloat)w,
			16.0 * (GLfloat)h / (GLfloat)w, -16.0, 16.0, -ZVALUE, ZVALUE);
	glMatrixMode(GL_MODELVIEW); //开始设置模型视点变换矩阵
	glLoadIdentity();//重置模型视点变换矩阵
}

//主调函数
int main(int argc, char** argv)
{
	glutInit(&argc, argv);//初始化GLUT库
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);//初始化显示模式
	glutInitWindowSize(500, 500);//设置窗口的大小
	glutInitWindowPosition(0, 0);//设置窗口的位置
	glutCreateWindow("100分！！！wasd ijkl移动 qe uo旋转");//创建窗口

	glutReshapeFunc(reshape);//设置窗口尺寸变换时对应的回调函数
	glutDisplayFunc(display);//设置绘制时对应的回调函数
	glutKeyboardFunc(keyboard);//设置键盘交互时对应的回调函数
	glutMouseFunc(mouse);//设置鼠标交互时对应的回调函数
	glutMainLoop();//启动图形程序
	return 0;
}