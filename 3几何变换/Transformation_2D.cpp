#define GLUT_DISABLE_ATEXIT_HACK
#include "GLUT.H"
#include <math.h>
#include <string.h> 

#define ZVALUE 20.0f
#define pi 3.1415926535

int w_width = 600; //视口、窗口的宽度
int w_height = 600; //视口、窗口的高度

//自定义欧氏平面点和向量
struct my_v_inhomogeneous
{
	double x;
	double y;
};

//齐次坐标下的点和向量
struct my_v_homogeneous
{
	double x;
	double y;
	double ratio;
};
 
//多边形顶点坐标
//每条边都是直线
struct my_v_inhomogeneous rectangle[4];
struct my_v_inhomogeneous triangle1[3], triangle2[3], triangle3[3];

//初始化多边形顶点坐标
void init(void)
{
	rectangle[0].x = 50;
	rectangle[0].y = 50;

	rectangle[1].x = 130;
	rectangle[1].y = 50;

	rectangle[2].x = 130;
	rectangle[2].y = 90;

	rectangle[3].x = 50;
	rectangle[3].y = 90;
	
	triangle1[0].x = 0;
	triangle1[0].y = 200;
	triangle1[1].x = -100 * sqrt(3);
	triangle1[1].y = -100;
	triangle1[2].x = 100 * sqrt(3);
	triangle1[2].y = -100;

	triangle2[0].x = 0;
	triangle2[0].y = 200;
	triangle2[1].x = -100 * sqrt(3);
	triangle2[1].y = -100;
	triangle2[2].x = 100 * sqrt(3);
	triangle2[2].y = -100;

	triangle3[0].x = 0;
	triangle3[0].y = 200;
	triangle3[1].x = -100 * sqrt(3);
	triangle3[1].y = -100;
	triangle3[2].x = 100 * sqrt(3);
	triangle3[2].y = -100;
}

//绘制坐标系
void draw_coordinate()
{
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0); //设置接下来显示的颜色
		glVertex2f(w_width, 0.0);
		glVertex2f(-w_width, 0.0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex2f(0.0, w_height);
		glVertex2f(0.0, -w_height);
	glEnd();
}

//绘制内容
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_coordinate(); //绘制坐标系

	glColor3f(157.0 / 256, 195.0 / 256, 230.0 / 256);
	//绘制多边形,默认顶点之间通过直线段相连
	/*
	glBegin(GL_POLYGON); //GL_LINE_LOOP
		for(int vIndex = 0;vIndex<3;vIndex ++)
		{
			glVertex2i(rectangle[vIndex].x,rectangle[vIndex].y);
			glVertex2i(rectangle[vIndex+1].x,rectangle[vIndex+1].y);
		}
	glEnd();
	*/
	//加黑多边形绘制边框
	glColor3f(0, 0, 0);
	/*
	glBegin(GL_LINE_LOOP);
	for (int vIndex = 0; vIndex < 3; vIndex++)
	{
		glVertex2i(rectangle[vIndex].x, rectangle[vIndex].y);
		glVertex2i(rectangle[vIndex + 1].x, rectangle[vIndex + 1].y);
	}
	glEnd();
	*/
	glBegin(GL_LINE_LOOP);
	for (int vIndex = 0; vIndex < 2; vIndex++)
	{
		glVertex2f(triangle1[vIndex].x, triangle1[vIndex].y);
		glVertex2f(triangle1[vIndex + 1].x, triangle1[vIndex + 1].y);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (int vIndex = 0; vIndex < 2; vIndex++)
	{
		glVertex2f(triangle2[vIndex].x, triangle2[vIndex].y);
		glVertex2f(triangle2[vIndex + 1].x, triangle2[vIndex + 1].y);
	}
	glEnd();
	glBegin(GL_LINE_LOOP);
	for (int vIndex = 0; vIndex < 2; vIndex++)
	{
		glVertex2f(triangle3[vIndex].x, triangle3[vIndex].y);
		glVertex2f(triangle3[vIndex + 1].x, triangle3[vIndex + 1].y);
	}
	glEnd();
	glutSwapBuffers();
}
 
//矩阵向量相乘
struct my_v_homogeneous matrix_multiply_vector(double matrix[][3], struct my_v_homogeneous input_v)
{
	struct my_v_homogeneous translated_v;
	translated_v.x = matrix[0][0] * input_v.x + matrix[0][1] * input_v.y + matrix[0][2] * input_v.ratio;
	translated_v.y = matrix[1][0] * input_v.x + matrix[1][1] * input_v.y + matrix[1][2] * input_v.ratio;
	translated_v.ratio = matrix[2][0] * input_v.x + matrix[2][1] * input_v.y + matrix[2][2] * 1;
	return translated_v;
}

//非齐次平移
void my_traslate_inhomogeneous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count, int tx, int ty)
{
	for (int vIndex = 0;vIndex<polygon_vertex_count;vIndex++)
	{
		polygon[vIndex].x += tx;
		polygon[vIndex].y += ty;
	}
} 

//齐次平移
void my_traslate_homogeneous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count, int tx, int ty)
{
	//装配生成平移矩阵 
	double translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(double) * 9);
	translate_matrix[0][0] = translate_matrix[1][1] = translate_matrix[2][2] = 1;//对角线赋值为1
	translate_matrix[0][2] = tx;
	translate_matrix[1][2] = ty;
	 
	//遍历并平移多边形每个顶点
	for (int vIndex = 0; vIndex<polygon_vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v); //平移矩阵作用到每个顶点
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
	}
}

//非齐次旋转
void my_rotate_inhomogeneous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count, double angle)
{
	angle = pi * angle / 180;
	float x, y;
	for (int vIndex = 0; vIndex < polygon_vertex_count; vIndex++)
	{
		x = polygon[vIndex].x;
		y = polygon[vIndex].y;
		polygon[vIndex].x = x * cos(angle) - y * sin(angle);
		polygon[vIndex].y = x * sin(angle) + y * cos(angle);
	}
}

//齐次旋转
void my_rotate_homogeneous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count, double theta)
{
	theta = pi * theta / 180;
	//装配生成矩阵 
	double translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(double) * 9);
	translate_matrix[0][0] = translate_matrix[1][1] = cos(theta);
	translate_matrix[0][1] = -sin(theta);
	translate_matrix[1][0] = sin(theta);
	translate_matrix[2][2] = 1;

	//遍历多边形每个顶点
	for (int vIndex = 0; vIndex < polygon_vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v); //矩阵作用到每个顶点
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
	}
}

//非齐次缩放
void my_scale_inhomogeneous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count, double sx, double sy)
{
	for (int vIndex = 0; vIndex < polygon_vertex_count; vIndex++)
	{
		polygon[vIndex].x *= sx;
		polygon[vIndex].y *= sy;
	}
}

//齐次缩放
void my_scale_homogeneous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count, double sx, double sy)
{
	//装配生成矩阵 
	double translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(double) * 9);
	translate_matrix[0][0] = sx;
	translate_matrix[1][1] = sy;
	translate_matrix[2][2] = 1;

	//遍历多边形每个顶点
	for (int vIndex = 0; vIndex < polygon_vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v); //矩阵作用到每个顶点
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
	}
}

//复合矩阵
void my_fuhe_homogenenous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count)
{
	//装配生成矩阵 
	double translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(double) * 9);
	translate_matrix[0][0] = 0.5;
	translate_matrix[0][1] = sqrt(3) / 2;
	translate_matrix[0][2] = 10;
	translate_matrix[1][0] = -sqrt(3) / 2;
	translate_matrix[1][1] = 0.5;
	translate_matrix[1][2] = 5;
	translate_matrix[2][2] = 1;

	//遍历多边形每个顶点
	for (int vIndex = 0; vIndex < polygon_vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v); //矩阵作用到每个顶点
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
	}
}

//键盘交互事件
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case'1':
	{
		my_rotate_inhomogeneous(triangle1, 3, 60);
		my_scale_inhomogeneous(triangle1, 3, 1.0/4, 1.0/6);
		glutPostRedisplay();
		break;
	}
	case'2':
	{
		my_rotate_inhomogeneous(triangle2, 3, 60);
		my_scale_inhomogeneous(triangle2, 3, 1.0/6, 1.0/6);
		glutPostRedisplay();
		break;
	}
	case'3':
	{
		glutPostRedisplay();
		break;
	}
	case 'w':
	case 'W':
	{
		//my_traslate_inhomogeneous(rectangle, 4, 0, 1);//向Y轴正方向移动1个单元
		//my_traslate_homogeneous(rectangle, 4, 0, 5);//向Y轴正方向移动1个单元
		my_traslate_homogeneous(triangle1, 3, 0, 5);//向Y轴正方向移动1个单元
		glutPostRedisplay();
		break;
	}
	case 's':
	case 'S':
	{
		//my_traslate_inhomogeneous(rectangle, 4, 0, -1);//向Y轴负方向移动1个单元
		//my_traslate_homogeneous(rectangle, 4, 0, -5);//向Y轴负方向移动1个单元
		my_traslate_homogeneous(triangle1, 3, 0, -5);//向Y轴正方向移动1个单元
		glutPostRedisplay();
		break;
	}
	case 'a':
	case 'A':
	{
		//my_traslate_inhomogeneous(rectangle, 4, -1, 0);//向X轴负方向移动1个单元
		//my_traslate_homogeneous(rectangle, 4, -10, 0);//向X轴负方向移动1个单元
		my_traslate_homogeneous(triangle1, 3, -10, 0);//向Y轴正方向移动1个单元
		glutPostRedisplay();
		break;
	}
	case 'd':
	case 'D':
	{
		//my_traslate_inhomogeneous(rectangle, 4, 1, 0);//向X轴正方向移动1个单元
		//my_traslate_homogeneous(rectangle, 4, 10, 0); //向X轴正方向移动1个单元
		my_traslate_homogeneous(triangle1, 3, 10, 0);//向Y轴正方向移动1个单元
		glutPostRedisplay();
		break;
	}

	case 'i':
	case 'I':
	{
		//my_scale_inhomogeneous(rectangle, 4, 2, 3);
		my_scale_homogeneous(rectangle, 4, 2, 3);
		my_scale_homogeneous(triangle1, 3, 2, 3);
		glutPostRedisplay();
		break;
	}
	case 'k':
	case 'K':
	{
		//my_scale_inhomogeneous(rectangle, 4, 0.5, 0.3);
		my_scale_homogeneous(rectangle, 4, 0.5, 0.3333333333333333333333333333333333333333333333333333);
		my_scale_homogeneous(triangle1, 3, 0.5, 0.3333333333333333333333333333333333333333333333333333);
		glutPostRedisplay();
		break;
	}
	case 'j':
	case 'J':
	{
		//my_rotate_inhomogeneous(rectangle, 4, 30);
		my_rotate_homogeneous(rectangle, 4, 60);
		my_rotate_homogeneous(triangle1, 3, 60);
		glutPostRedisplay();
		break;
	}
	case 'l':
	case 'L':
	{
		//my_rotate_inhomogeneous(rectangle, 4, -30);
		my_rotate_homogeneous(rectangle, 4, -60);
		my_rotate_homogeneous(triangle1, 3, -60);
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
		if (state == GLUT_DOWN) 
		{ 
			//my_traslate_inhomogeneous(rectangle, 4, 1, 1); //沿X和Y轴正方向同时移动1个单元
			my_traslate_homogeneous(rectangle, 4, 1, 1);     //沿X和Y轴正方向同时移动1个单元
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) 
		{ 
			//my_traslate_inhomogeneous(rectangle, 4, -1, -1); //沿X和Y轴负方向同时移动1个单元
			my_traslate_homogeneous(rectangle, 4, -1, -1);     //沿X和Y轴负方向同时移动1个单元
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

//投影方式、modelview方式设置
void reshape(int w, int h)
{   
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*if (w <= h) 
		glOrtho(-16.0, 16.0, -16.0*(GLfloat)h/(GLfloat)w, 16.0*(GLfloat)h/(GLfloat)w,
		-ZVALUE, ZVALUE);
	else
		glOrtho(-16.0*(GLfloat)h/(GLfloat)w, 16.0*(GLfloat)h/(GLfloat)w, -16.0, 16.0,
		-ZVALUE, ZVALUE);*/

	if (w <= h)
		glOrtho(-0.5 * w_width, 0.5 * w_width, -0.5 * w_height * (GLfloat)w_height / (GLfloat)w_width, 0.5 * w_height * (GLfloat)w_height / (GLfloat)w_width,
			-ZVALUE, ZVALUE);
	else
		glOrtho(-0.5 * w_width, 0.5 * w_width, -0.5 * w_height * (GLfloat)w_width / (GLfloat)w_height, 0.5 * w_height * (GLfloat)w_width / (GLfloat)w_height,
			-ZVALUE, ZVALUE);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//主调函数
int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(w_width, w_height);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("按1实施C1 按2实施C2");

   init();

   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc (keyboard);
   glutMouseFunc(mouse);
   glutMainLoop();
   return 0;
}

 
