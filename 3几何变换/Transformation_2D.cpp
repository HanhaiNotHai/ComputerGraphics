#define GLUT_DISABLE_ATEXIT_HACK
#include "GLUT.H"
#include <math.h>
#include <string.h> 

#define ZVALUE 20.0f
#define pi 3.1415926535

int w_width = 600; //�ӿڡ����ڵĿ��
int w_height = 600; //�ӿڡ����ڵĸ߶�

//�Զ���ŷ��ƽ��������
struct my_v_inhomogeneous
{
	double x;
	double y;
};

//��������µĵ������
struct my_v_homogeneous
{
	double x;
	double y;
	double ratio;
};
 
//����ζ�������
//ÿ���߶���ֱ��
struct my_v_inhomogeneous rectangle[4];
struct my_v_inhomogeneous triangle1[3], triangle2[3], triangle3[3];

//��ʼ������ζ�������
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

//��������
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_coordinate(); //��������ϵ

	glColor3f(157.0 / 256, 195.0 / 256, 230.0 / 256);
	//���ƶ����,Ĭ�϶���֮��ͨ��ֱ�߶�����
	/*
	glBegin(GL_POLYGON); //GL_LINE_LOOP
		for(int vIndex = 0;vIndex<3;vIndex ++)
		{
			glVertex2i(rectangle[vIndex].x,rectangle[vIndex].y);
			glVertex2i(rectangle[vIndex+1].x,rectangle[vIndex+1].y);
		}
	glEnd();
	*/
	//�Ӻڶ���λ��Ʊ߿�
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
 
//�����������
struct my_v_homogeneous matrix_multiply_vector(double matrix[][3], struct my_v_homogeneous input_v)
{
	struct my_v_homogeneous translated_v;
	translated_v.x = matrix[0][0] * input_v.x + matrix[0][1] * input_v.y + matrix[0][2] * input_v.ratio;
	translated_v.y = matrix[1][0] * input_v.x + matrix[1][1] * input_v.y + matrix[1][2] * input_v.ratio;
	translated_v.ratio = matrix[2][0] * input_v.x + matrix[2][1] * input_v.y + matrix[2][2] * 1;
	return translated_v;
}

//�����ƽ��
void my_traslate_inhomogeneous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count, int tx, int ty)
{
	for (int vIndex = 0;vIndex<polygon_vertex_count;vIndex++)
	{
		polygon[vIndex].x += tx;
		polygon[vIndex].y += ty;
	}
} 

//���ƽ��
void my_traslate_homogeneous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count, int tx, int ty)
{
	//װ������ƽ�ƾ��� 
	double translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(double) * 9);
	translate_matrix[0][0] = translate_matrix[1][1] = translate_matrix[2][2] = 1;//�Խ��߸�ֵΪ1
	translate_matrix[0][2] = tx;
	translate_matrix[1][2] = ty;
	 
	//������ƽ�ƶ����ÿ������
	for (int vIndex = 0; vIndex<polygon_vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v); //ƽ�ƾ������õ�ÿ������
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
	}
}

//�������ת
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

//�����ת
void my_rotate_homogeneous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count, double theta)
{
	theta = pi * theta / 180;
	//װ�����ɾ��� 
	double translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(double) * 9);
	translate_matrix[0][0] = translate_matrix[1][1] = cos(theta);
	translate_matrix[0][1] = -sin(theta);
	translate_matrix[1][0] = sin(theta);
	translate_matrix[2][2] = 1;

	//���������ÿ������
	for (int vIndex = 0; vIndex < polygon_vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v); //�������õ�ÿ������
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
	}
}

//���������
void my_scale_inhomogeneous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count, double sx, double sy)
{
	for (int vIndex = 0; vIndex < polygon_vertex_count; vIndex++)
	{
		polygon[vIndex].x *= sx;
		polygon[vIndex].y *= sy;
	}
}

//�������
void my_scale_homogeneous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count, double sx, double sy)
{
	//װ�����ɾ��� 
	double translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(double) * 9);
	translate_matrix[0][0] = sx;
	translate_matrix[1][1] = sy;
	translate_matrix[2][2] = 1;

	//���������ÿ������
	for (int vIndex = 0; vIndex < polygon_vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v); //�������õ�ÿ������
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
	}
}

//���Ͼ���
void my_fuhe_homogenenous(struct my_v_inhomogeneous* polygon, int polygon_vertex_count)
{
	//װ�����ɾ��� 
	double translate_matrix[3][3];
	memset(translate_matrix, 0, sizeof(double) * 9);
	translate_matrix[0][0] = 0.5;
	translate_matrix[0][1] = sqrt(3) / 2;
	translate_matrix[0][2] = 10;
	translate_matrix[1][0] = -sqrt(3) / 2;
	translate_matrix[1][1] = 0.5;
	translate_matrix[1][2] = 5;
	translate_matrix[2][2] = 1;

	//���������ÿ������
	for (int vIndex = 0; vIndex < polygon_vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v); //�������õ�ÿ������
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
	}
}

//���̽����¼�
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
		//my_traslate_inhomogeneous(rectangle, 4, 0, 1);//��Y���������ƶ�1����Ԫ
		//my_traslate_homogeneous(rectangle, 4, 0, 5);//��Y���������ƶ�1����Ԫ
		my_traslate_homogeneous(triangle1, 3, 0, 5);//��Y���������ƶ�1����Ԫ
		glutPostRedisplay();
		break;
	}
	case 's':
	case 'S':
	{
		//my_traslate_inhomogeneous(rectangle, 4, 0, -1);//��Y�Ḻ�����ƶ�1����Ԫ
		//my_traslate_homogeneous(rectangle, 4, 0, -5);//��Y�Ḻ�����ƶ�1����Ԫ
		my_traslate_homogeneous(triangle1, 3, 0, -5);//��Y���������ƶ�1����Ԫ
		glutPostRedisplay();
		break;
	}
	case 'a':
	case 'A':
	{
		//my_traslate_inhomogeneous(rectangle, 4, -1, 0);//��X�Ḻ�����ƶ�1����Ԫ
		//my_traslate_homogeneous(rectangle, 4, -10, 0);//��X�Ḻ�����ƶ�1����Ԫ
		my_traslate_homogeneous(triangle1, 3, -10, 0);//��Y���������ƶ�1����Ԫ
		glutPostRedisplay();
		break;
	}
	case 'd':
	case 'D':
	{
		//my_traslate_inhomogeneous(rectangle, 4, 1, 0);//��X���������ƶ�1����Ԫ
		//my_traslate_homogeneous(rectangle, 4, 10, 0); //��X���������ƶ�1����Ԫ
		my_traslate_homogeneous(triangle1, 3, 10, 0);//��Y���������ƶ�1����Ԫ
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

//��꽻���¼�
void mouse(int button, int state, int x, int y)
{
	switch (button) 
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) 
		{ 
			//my_traslate_inhomogeneous(rectangle, 4, 1, 1); //��X��Y��������ͬʱ�ƶ�1����Ԫ
			my_traslate_homogeneous(rectangle, 4, 1, 1);     //��X��Y��������ͬʱ�ƶ�1����Ԫ
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) 
		{ 
			//my_traslate_inhomogeneous(rectangle, 4, -1, -1); //��X��Y�Ḻ����ͬʱ�ƶ�1����Ԫ
			my_traslate_homogeneous(rectangle, 4, -1, -1);     //��X��Y�Ḻ����ͬʱ�ƶ�1����Ԫ
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
}

//ͶӰ��ʽ��modelview��ʽ����
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

//��������
int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(w_width, w_height);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("��1ʵʩC1 ��2ʵʩC2");

   init();

   glutReshapeFunc(reshape);
   glutDisplayFunc(display);
   glutKeyboardFunc (keyboard);
   glutMouseFunc(mouse);
   glutMainLoop();
   return 0;
}

 
