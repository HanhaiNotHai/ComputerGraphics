#define GLUT_DISABLE_ATEXIT_HACK
double pi = 3.1415926535;

#include<stdio.h>
#include "GLUT.H"
#include <math.h>
#include <string.h>

int nearplane_width = 600; //�Ӿ�����
int nearplane_height = 600; //�Ӿ���߶�
int nearplane_distance = 50; //�Ӿ����ƽ�����ӵ����
int farplane_distance = nearplane_distance + 300; //�Ӿ���Զƽ�����ӵ����

int eye_x = 80; //�ӵ��x����

//��ά��������µĵ������
struct my_v_homogeneous
{
	double x;
	double y;
	double z;
	double ratio;
};

//box�Ķ�������
struct my_v_homogeneous box[2][8];

//��ʼ�������嶥������
void init(void)
{
	//box[0]
	//ǰ���ĸ�����
	box[0][0].x = 0;
	box[0][0].y = 0;
	box[0][0].z = 0;
	box[0][0].ratio = 1;

	box[0][1].x = 80;
	box[0][1].y = 0;
	box[0][1].z = 0;
	box[0][1].ratio = 1;

	box[0][2].x = 80;
	box[0][2].y = 40;
	box[0][2].z = 0;
	box[0][2].ratio = 1;

	box[0][3].x = 0;
	box[0][3].y = 40;
	box[0][3].z = 0;
	box[0][3].ratio = 1;

	//�����ĸ�����
	box[0][4].x = 0;
	box[0][4].y = 0;
	box[0][4].z = -50;
	box[0][4].ratio = 1;

	box[0][5].x = 80;
	box[0][5].y = 0;
	box[0][5].z = -50;
	box[0][5].ratio = 1;

	box[0][6].x = 80;
	box[0][6].y = 40;
	box[0][6].z = -50;
	box[0][6].ratio = 1;

	box[0][7].x = 0;
	box[0][7].y = 40;
	box[0][7].z = -50;
	box[0][7].ratio = 1;

	//box[1]
	//�����ĸ�����
	box[1][0].x = 0;
	box[1][0].y = 0;
	box[1][0].z = 0;
	box[1][0].ratio = 1;

	box[1][1].x = -80;
	box[1][1].y = 0;
	box[1][1].z = 0;
	box[1][1].ratio = 1;

	box[1][2].x = -80;
	box[1][2].y = 40;
	box[1][2].z = 0;
	box[1][2].ratio = 1;

	box[1][3].x = 0;
	box[1][3].y = 40;
	box[1][3].z = 0;
	box[1][3].ratio = 1;

	//ǰ���ĸ�����
	box[1][4].x = 0;
	box[1][4].y = 0;
	box[1][4].z = 50;
	box[1][4].ratio = 1;

	box[1][5].x = -80;
	box[1][5].y = 0;
	box[1][5].z = 50;
	box[1][5].ratio = 1;

	box[1][6].x = -80;
	box[1][6].y = 40;
	box[1][6].z = 50;
	box[1][6].ratio = 1;

	box[1][7].x = 0;
	box[1][7].y = 40;
	box[1][7].z = 50;
	box[1][7].ratio = 1;
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

//�������� ���Ƴ�����
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_coordinate(); //��������ϵ

	//����box,Ĭ�϶���֮��ͨ��ֱ�߶�����
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	//box[0]
	//ǰ��4����
	glVertex3i((floor)(box[0][0].x + 0.5), (floor)(box[0][0].y + 0.5), (floor)(box[0][0].z + 0.5));
	glVertex3i((floor)(box[0][1].x + 0.5), (floor)(box[0][1].y + 0.5), (floor)(box[0][1].z + 0.5));
	glVertex3i((floor)(box[0][1].x + 0.5), (floor)(box[0][1].y + 0.5), (floor)(box[0][1].z + 0.5));
	glVertex3i((floor)(box[0][2].x + 0.5), (floor)(box[0][2].y + 0.5), (floor)(box[0][2].z + 0.5));
	glVertex3i((floor)(box[0][2].x + 0.5), (floor)(box[0][2].y + 0.5), (floor)(box[0][2].z + 0.5));
	glVertex3i((floor)(box[0][3].x + 0.5), (floor)(box[0][3].y + 0.5), (floor)(box[0][3].z + 0.5));
	glVertex3i((floor)(box[0][3].x + 0.5), (floor)(box[0][3].y + 0.5), (floor)(box[0][3].z + 0.5));
	glVertex3i((floor)(box[0][0].x + 0.5), (floor)(box[0][0].y + 0.5), (floor)(box[0][0].z + 0.5));
	//����4����
	glVertex3i((floor)(box[0][4].x + 0.5), (floor)(box[0][4].y + 0.5), (floor)(box[0][4].z + 0.5));
	glVertex3i((floor)(box[0][5].x + 0.5), (floor)(box[0][5].y + 0.5), (floor)(box[0][5].z + 0.5));
	glVertex3i((floor)(box[0][5].x + 0.5), (floor)(box[0][5].y + 0.5), (floor)(box[0][5].z + 0.5));
	glVertex3i((floor)(box[0][6].x + 0.5), (floor)(box[0][6].y + 0.5), (floor)(box[0][6].z + 0.5));
	glVertex3i((floor)(box[0][6].x + 0.5), (floor)(box[0][6].y + 0.5), (floor)(box[0][6].z + 0.5));
	glVertex3i((floor)(box[0][7].x + 0.5), (floor)(box[0][7].y + 0.5), (floor)(box[0][7].z + 0.5));
	glVertex3i((floor)(box[0][7].x + 0.5), (floor)(box[0][7].y + 0.5), (floor)(box[0][7].z + 0.5));
	glVertex3i((floor)(box[0][4].x + 0.5), (floor)(box[0][4].y + 0.5), (floor)(box[0][4].z + 0.5));
	//���油������
	glVertex3i((floor)(box[0][0].x + 0.5), (floor)(box[0][0].y + 0.5), (floor)(box[0][0].z + 0.5));
	glVertex3i((floor)(box[0][4].x + 0.5), (floor)(box[0][4].y + 0.5), (floor)(box[0][4].z + 0.5));
	glVertex3i((floor)(box[0][7].x + 0.5), (floor)(box[0][7].y + 0.5), (floor)(box[0][7].z + 0.5));
	glVertex3i((floor)(box[0][3].x + 0.5), (floor)(box[0][3].y + 0.5), (floor)(box[0][3].z + 0.5));
	//���油������
	glVertex3i((floor)(box[0][1].x + 0.5), (floor)(box[0][1].y + 0.5), (floor)(box[0][1].z + 0.5));
	glVertex3i((floor)(box[0][5].x + 0.5), (floor)(box[0][5].y + 0.5), (floor)(box[0][5].z + 0.5));
	glVertex3i((floor)(box[0][6].x + 0.5), (floor)(box[0][6].y + 0.5), (floor)(box[0][6].z + 0.5));
	glVertex3i((floor)(box[0][2].x + 0.5), (floor)(box[0][2].y + 0.5), (floor)(box[0][2].z + 0.5));
	
	////box[1]
	////ǰ��4����
	//glVertex3i((floor)(box[1][0].x + 0.5), (floor)(box[1][0].y + 0.5), (floor)(box[1][0].z + 0.5));
	//glVertex3i((floor)(box[1][1].x + 0.5), (floor)(box[1][1].y + 0.5), (floor)(box[1][1].z + 0.5));
	//glVertex3i((floor)(box[1][1].x + 0.5), (floor)(box[1][1].y + 0.5), (floor)(box[1][1].z + 0.5));
	//glVertex3i((floor)(box[1][2].x + 0.5), (floor)(box[1][2].y + 0.5), (floor)(box[1][2].z + 0.5));
	//glVertex3i((floor)(box[1][2].x + 0.5), (floor)(box[1][2].y + 0.5), (floor)(box[1][2].z + 0.5));
	//glVertex3i((floor)(box[1][3].x + 0.5), (floor)(box[1][3].y + 0.5), (floor)(box[1][3].z + 0.5));
	//glVertex3i((floor)(box[1][3].x + 0.5), (floor)(box[1][3].y + 0.5), (floor)(box[1][3].z + 0.5));
	//glVertex3i((floor)(box[1][0].x + 0.5), (floor)(box[1][0].y + 0.5), (floor)(box[1][0].z + 0.5));
	////����4����
	//glVertex3i((floor)(box[1][4].x + 0.5), (floor)(box[1][4].y + 0.5), (floor)(box[1][4].z + 0.5));
	//glVertex3i((floor)(box[1][5].x + 0.5), (floor)(box[1][5].y + 0.5), (floor)(box[1][5].z + 0.5));
	//glVertex3i((floor)(box[1][5].x + 0.5), (floor)(box[1][5].y + 0.5), (floor)(box[1][5].z + 0.5));
	//glVertex3i((floor)(box[1][6].x + 0.5), (floor)(box[1][6].y + 0.5), (floor)(box[1][6].z + 0.5));
	//glVertex3i((floor)(box[1][6].x + 0.5), (floor)(box[1][6].y + 0.5), (floor)(box[1][6].z + 0.5));
	//glVertex3i((floor)(box[1][7].x + 0.5), (floor)(box[1][7].y + 0.5), (floor)(box[1][7].z + 0.5));
	//glVertex3i((floor)(box[1][7].x + 0.5), (floor)(box[1][7].y + 0.5), (floor)(box[1][7].z + 0.5));
	//glVertex3i((floor)(box[1][4].x + 0.5), (floor)(box[1][4].y + 0.5), (floor)(box[1][4].z + 0.5));
	////���油������
	//glVertex3i((floor)(box[1][0].x + 0.5), (floor)(box[1][0].y + 0.5), (floor)(box[1][0].z + 0.5));
	//glVertex3i((floor)(box[1][4].x + 0.5), (floor)(box[1][4].y + 0.5), (floor)(box[1][4].z + 0.5));
	//glVertex3i((floor)(box[1][7].x + 0.5), (floor)(box[1][7].y + 0.5), (floor)(box[1][7].z + 0.5));
	//glVertex3i((floor)(box[1][3].x + 0.5), (floor)(box[1][3].y + 0.5), (floor)(box[1][3].z + 0.5));
	////���油������
	//glVertex3i((floor)(box[1][1].x + 0.5), (floor)(box[1][1].y + 0.5), (floor)(box[1][1].z + 0.5));
	//glVertex3i((floor)(box[1][5].x + 0.5), (floor)(box[1][5].y + 0.5), (floor)(box[1][5].z + 0.5));
	//glVertex3i((floor)(box[1][6].x + 0.5), (floor)(box[1][6].y + 0.5), (floor)(box[1][6].z + 0.5));
	//glVertex3i((floor)(box[1][2].x + 0.5), (floor)(box[1][2].y + 0.5), (floor)(box[1][2].z + 0.5));
	glEnd();
	glutSwapBuffers();
}

//�����������
struct my_v_homogeneous matrix_multiply_vector(float matrix[][4], struct my_v_homogeneous input_v)
{
	struct my_v_homogeneous translated_v;
	translated_v.x = matrix[0][0] * input_v.x + matrix[0][1] * input_v.y + matrix[0][2] * input_v.z + matrix[0][3] * input_v.ratio;
	translated_v.y = matrix[1][0] * input_v.x + matrix[1][1] * input_v.y + matrix[1][2] * input_v.z + matrix[1][3] * input_v.ratio;
	translated_v.z = matrix[2][0] * input_v.x + matrix[2][1] * input_v.y + matrix[2][2] * input_v.z + matrix[2][3] * input_v.ratio;
	translated_v.ratio = matrix[3][0] * input_v.x + matrix[3][1] * input_v.y + matrix[3][2] * input_v.z + matrix[3][3] * input_v.ratio;
	return translated_v;
}

//���ƽ��
void my_translate_homogeneous(struct my_v_homogeneous* polygon, int vertex_count, float tx, float ty, float tz)
{
	//װ������ƽ�ƾ���
	float translate_matrix[4][4];
	memset(translate_matrix, 0, sizeof(int) * 16);

	translate_matrix[0][0] = translate_matrix[1][1] = translate_matrix[2][2] =translate_matrix[3][3] = 1;//�Խ��߸�ֵΪ1
	translate_matrix[0][3] = tx;
	translate_matrix[1][3] = ty;
	translate_matrix[2][3] = tz;

	//������ƽ�ƶ����ÿ������
	for (int vIndex = 0; vIndex < vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;

		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.z = polygon[vIndex].z;
		input_v.ratio = 1;

		input_v = matrix_multiply_vector(translate_matrix, polygon[vIndex]);

		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
		polygon[vIndex].z = input_v.z;
		polygon[vIndex].ratio = input_v.ratio;
	}
}

//�����ת
void my_rotate_homogeneous(struct my_v_homogeneous* polygon, int vertex_count, char axis, float angle)
{
	angle = angle * pi / 180;

	//װ�����ɱ任����
	float translate_matrix[4][4];
	memset(translate_matrix, 0, sizeof(int) * 16);

	switch (axis)
	{
	case'x':
	{
		translate_matrix[0][0] = translate_matrix[3][3] = 1;
		translate_matrix[1][1] = cos(angle);
		translate_matrix[2][2] = cos(angle);
		translate_matrix[1][2] = -sin(angle);
		translate_matrix[2][1] = sin(angle);
		break;
	}
	case'y':
	{
		translate_matrix[1][1] = translate_matrix[3][3] = 1;
		translate_matrix[2][2] = cos(angle);
		translate_matrix[0][0] = cos(angle);
		translate_matrix[2][0] = -sin(angle);
		translate_matrix[0][2] = sin(angle);
		break;
	}
	case'z':
	{
		translate_matrix[2][2] = translate_matrix[3][3] = 1;
		translate_matrix[0][0] = cos(angle);
		translate_matrix[1][1] = cos(angle);
		translate_matrix[0][1] = -sin(angle);
		translate_matrix[1][0] = sin(angle);
		break;
	}
	}

	//�������任�����ÿ������
	for (int vIndex = 0; vIndex < vertex_count; vIndex++)
	{
		struct my_v_homogeneous input_v;

		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.z = polygon[vIndex].z;
		input_v.ratio = 1;

		input_v = matrix_multiply_vector(translate_matrix, polygon[vIndex]);

		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
		polygon[vIndex].z = input_v.z;
		polygon[vIndex].ratio = input_v.ratio;
	}
}

//����������ת x,y,z ����a,b,c
void my_axis_rotate_homogeneous(struct my_v_homogeneous* polygon, int vertex_count, float x, float y, float z, double a, double b, double c, float angle)
{
	float angle1 = atan2(b, a) * 180 / pi;
	float angle2 = atan2(c, sqrt(a * a + c * c)) * 180 / pi;

	my_translate_homogeneous(polygon, vertex_count, -x, -y, -z);
	my_rotate_homogeneous(polygon, vertex_count, 'z', -angle1);
	my_rotate_homogeneous(polygon, vertex_count, 'y', angle2);

	my_rotate_homogeneous(polygon, vertex_count, 'x', angle);

	my_rotate_homogeneous(polygon, vertex_count, 'y', -angle2);
	my_rotate_homogeneous(polygon, vertex_count, 'z', angle1);
	my_translate_homogeneous(polygon, vertex_count, x, y, z);
}

void renyizhou(struct my_v_homogeneous* polygon, int vertex_count, float x0, float y0, float z0, double nx, double ny, double nz, float angle)
{
	float n, t, xc, yc, zc, r, translate_matrix[4][4];
	struct my_v_homogeneous op, y, temp;
	n = sqrt(nx * nx + ny * ny + nz * nz);
	nx /= n;
	ny /= n;
	nz/=n;
	angle = angle * pi / 180;
	for (int vIndex = 0; vIndex < vertex_count; vIndex++)
	{
		//x=x0+nx*t;y=y0+ny*t;z=z0+nz*t
		t = nx * (polygon[vIndex].x - x0)+ 
			ny * (polygon[vIndex].y - y0)+ 
			nz * (polygon[vIndex].z - z0);
		//Բ�� �뾶
		xc = x0 + nx * t;
		yc = y0 + ny * t;
		zc = z0 + nz * t;
		r = sqrt(
			(polygon[vIndex].x - xc) * (polygon[vIndex].x - xc) +
			(polygon[vIndex].y - yc) * (polygon[vIndex].y - yc) +
			(polygon[vIndex].z - zc) * (polygon[vIndex].z - zc));
		//����op ��x��
		op.x = (polygon[vIndex].x - xc) / r;
		op.y = (polygon[vIndex].y - yc) / r;
		op.z = (polygon[vIndex].z - zc) / r;
		//��y��
		y.x = ny * op.z - nz * op.y;
		y.y = nz * op.x - nx * op.z;
		y.z = nx * op.y - ny * op.z;
		//��ת�����������ϵ������
		temp.x = r * cos(angle);
		temp.y = r * sin(angle);
		temp.z = 0;
		temp.ratio = 1;
		//װ�����
		/*memset(translate_matrix, 0, sizeof(int) * 16);

		translate_matrix[0][0] = op.x;
		translate_matrix[1][0] = op.y;
		translate_matrix[2][0] = op.z;

		translate_matrix[0][1] = y.x;
		translate_matrix[1][1] = y.y;
		translate_matrix[2][1] = y.z;

		translate_matrix[0][2] = nx;
		translate_matrix[1][2] = ny;
		translate_matrix[2][2] = nz;

		translate_matrix[0][3] = xc;
		translate_matrix[1][3] = yc;
		translate_matrix[2][3] = zc;
		translate_matrix[3][3] = 1;

		polygon[vIndex] = matrix_multiply_vector(translate_matrix, temp);*/

		polygon[vIndex].x = op.x * temp.x + y.x * temp.y + xc;
		polygon[vIndex].y = op.y * temp.x + y.y * temp.y + yc;
		polygon[vIndex].z = op.z * temp.x + y.z * temp.y + zc;
	}
}

//void my_chaode_rotate_homogeneous(struct my_v_homogeneous* polygon, int vertex_count, float x, float y, float z, float a, float b, float c, float angle)
//{
//	angle = angle * pi / 180;
//	float k = 1 - cos(angle);
//	float m = a * x + b * y + c * z;
//
//	//װ������ƽ�ƾ���
//	float translate_matrix[4][4];
//	memset(translate_matrix, 0, sizeof(int) * 16);
//
//	translate_matrix[0][0] = a * a * k + cos(angle);
//	translate_matrix[0][1] = a * b * k - c * sin(angle);
//	translate_matrix[0][2] = a * c * k + b * sin(angle);
//	translate_matrix[0][3] = (x - a * m) * k + (c * y - b * z) * sin(angle);
//
//	translate_matrix[1][0] = a * b * k + c * sin(angle);
//	translate_matrix[1][1] = b * b * k + cos(angle);
//	translate_matrix[1][2] = b * c * k - a * sin(angle);
//	translate_matrix[1][3] = (y - b * m) * k + (a * z - c * x) * sin(angle);
//
//	translate_matrix[2][0] = a * c * k - b * sin(angle);
//	translate_matrix[2][1] = b * c * k + a * sin(angle);
//	translate_matrix[2][2] = c * c + cos(angle);
//	translate_matrix[2][3] = (z - m * z) * k + (b * x - a * y) * sin(angle);
//
//	translate_matrix[3][0] = 0;
//	translate_matrix[3][1] = 0;
//	translate_matrix[3][2] = 0;
//	translate_matrix[3][3] = 1;
//
//	//������ƽ�ƶ����ÿ������
//	for (int vIndex = 0; vIndex < vertex_count; vIndex++)
//	{
//		struct my_v_homogeneous input_v;
//
//		input_v.x = polygon[vIndex].x;
//		input_v.y = polygon[vIndex].y;
//		input_v.z = polygon[vIndex].z;
//		input_v.ratio = 1;
//
//		input_v = matrix_multiply_vector(translate_matrix, polygon[vIndex]);
//
//		polygon[vIndex].x = input_v.x;
//		polygon[vIndex].y = input_v.y;
//		polygon[vIndex].z = input_v.z;
//		polygon[vIndex].ratio = input_v.ratio;
//	}
//}

//���̽����¼�
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case'1':
	{
		//my_axis_rotate_homogeneous(box[0], 8, box[0][0].x, box[0][0].y, box[0][0].z,
		//	box[0][1].x - box[0][0].x, box[0][1].y - box[0][0].y, box[0][1].z - box[0][0].z, 30);
		my_axis_rotate_homogeneous(box[0], 8, 1, 1, 1, 1, 1, 1, 30);

		//my_axis_rotate_homogeneous(box[0], 8, 20, 20, 20, 1, 1, 1, 30);
		//my_axis_rotate_homogeneous(box[1], 8, 20, 20, 20, 1, 1, 1, 30);
		glutPostRedisplay();
		break;
	}
	case'2':
	{
		//renyizhou(box[0], 8, box[0][0].x, box[0][0].y, box[0][0].z,
		//	box[0][1].x - box[0][0].x, box[0][1].y - box[0][0].y, box[0][1].z - box[0][0].z, 30);
		renyizhou(box[0], 8, 1, 1, 1, 1, 1, 1, 1);
		
		//my_axis_rotate_homogeneous(box[1], 8, box[1][0].x, box[1][0].y, box[1][0].z,
		//	box[1][3].x - box[1][0].x, box[1][3].y - box[1][0].y, box[1][3].z - box[1][0].z, 30);
		glutPostRedisplay();
		break;
	}
	case'w':
	{
		my_translate_homogeneous(box[0], 8, 0, 10, 0);
		glutPostRedisplay();
		break;
	}
	case's':
	{
		my_translate_homogeneous(box[0], 8, 0, -10, 0);
		glutPostRedisplay();
		break;
	}
	case'a':
	{
		my_translate_homogeneous(box[0], 8, -10, 0, 0);
		glutPostRedisplay();
		break;
	}
	case'd':
	{
		my_translate_homogeneous(box[0], 8, 10, 0, 0);
		glutPostRedisplay();
		break;
	}
	case'e':
	{
		my_translate_homogeneous(box[0], 8, 0, 0, -10);
		glutPostRedisplay();
		break;
	}
	case'q':
	{
		my_translate_homogeneous(box[0], 8, 0, 0, 10);
		glutPostRedisplay();
		break;
	}
	case'l':
	{
		my_rotate_homogeneous(box[0], 8, 'x', 30);
		glutPostRedisplay();
		break;
	}
	case'j':
	{
		my_rotate_homogeneous(box[0], 8, 'x', -30);
		glutPostRedisplay();
		break;
	}
	case'i':
	{
		my_rotate_homogeneous(box[0], 8, 'y', 30);
		glutPostRedisplay();
		break;
	}
	case'k':
	{
		my_rotate_homogeneous(box[0], 8, 'y', -30);
		glutPostRedisplay();
		break;
	}
	case'u':
	{
		my_rotate_homogeneous(box[0], 8, 'z', 30);
		glutPostRedisplay();
		break;
	}
	case'o':
	{
		my_rotate_homogeneous(box[0], 8, 'z', -30);
		glutPostRedisplay();
		break;
	}
	case 'T':
	case 't':
	{
		my_translate_homogeneous(box[0], 8, 30, 0, 0);
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

	if (w <= h) //ƽ��ͶӰ
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height *
			(GLfloat)nearplane_height / (GLfloat)nearplane_width, 0.5 * nearplane_height *
			(GLfloat)nearplane_height / (GLfloat)nearplane_width,
			-nearplane_distance, farplane_distance); //������ӵ�
	else
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height *
			(GLfloat)nearplane_width / (GLfloat)nearplane_height, 0.5 * nearplane_height *
			(GLfloat)nearplane_width / (GLfloat)nearplane_height,
			-nearplane_distance, farplane_distance);

	//if (w <= h)
	//	glFrustum(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width, 0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width,
	//		nearplane_distance, farplane_distance); //������ӵ�
	//else
	//	glFrustum(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height * (GLfloat)nearplane_width / (GLfloat)nearplane_height, 0.5 * nearplane_height * (GLfloat)nearplane_width / (GLfloat)nearplane_height,
	//		nearplane_distance, farplane_distance);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_x, 80, 80, 0, 0, 0, 0, 1, 0);//�ӵ�ת��
}

//��꽻���¼�
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			eye_x += 10000;
			reshape(nearplane_width, nearplane_height);
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			eye_x -= 1;
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
	glutCreateWindow("��1ABһ��ת ��2Bת");

	init();
	
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}