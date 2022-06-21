#define GLUT_DISABLE_ATEXIT_HACK
#include "GLUT.H"
#include <math.h>
#include <string.h> 
#include <stdio.h>
#include "My_3DVector.h"

int nearplane_width = 600; //�Ӿ�����
int nearplane_height = 600; //�Ӿ���߶�
int nearplane_distance = 50; //�Ӿ����ƽ�����ӵ����
int farplane_distance = nearplane_distance + 300; //�Ӿ���Զƽ�����ӵ����
   
//���Զ���
my_homogeneous_point test_point;

//�ü�����ζ�������
//ÿ���߶���ֱ��
#define clipping_polygon_point_count 4
struct my_homogeneous_point clipping_polygon[clipping_polygon_point_count];

//���ü�����ζ�������
//ÿ���߶���ֱ��
struct my_homogeneous_point subject_polygon[4];

//��ʼ�������ζ�������
void init(void)
{
	test_point.x = 50;
	test_point.y = 50;
	test_point.z = 0;

	//ǰ���ĸ�����
	clipping_polygon[0].x = 0;
	clipping_polygon[0].y = 0;
	clipping_polygon[0].z = 0;
	clipping_polygon[0].ratio = 1;

	clipping_polygon[1].x = 180;
	clipping_polygon[1].y = 0;
	clipping_polygon[1].z = 0;
	clipping_polygon[1].ratio = 1;

	clipping_polygon[2].x = 180;
	clipping_polygon[2].y = 140;
	clipping_polygon[2].z = 0;
	clipping_polygon[2].ratio = 1;

	clipping_polygon[3].x = 0;
	clipping_polygon[3].y = 140;
	clipping_polygon[3].z = 0;
	clipping_polygon[3].ratio = 1;
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
	glEnd();
}

//////////////////////////////////////////////���ƽ�Ʊ任-���躯��//////////////////////////////////////////////
//�����������
struct my_homogeneous_point matrix_multiply_vector(float matrix[][4], struct my_homogeneous_point input_v)
{
	struct my_homogeneous_point translated_v;
	translated_v.x = matrix[0][0] * input_v.x + matrix[0][1] * input_v.y + matrix[0][2] * input_v.z + matrix[0][3] * input_v.ratio;
	translated_v.y = matrix[1][0] * input_v.x + matrix[1][1] * input_v.y + matrix[1][2] * input_v.z + matrix[1][3] * input_v.ratio;
	translated_v.z = matrix[2][0] * input_v.x + matrix[2][1] * input_v.y + matrix[2][2] * input_v.z + matrix[2][3] * input_v.ratio;
	translated_v.ratio = matrix[3][0] * input_v.x + matrix[3][1] * input_v.y + matrix[3][2] * input_v.z + matrix[3][3] * input_v.ratio;
	return translated_v;
}

//���ƽ�Ʊ任
void my_traslate_homogeneous(struct my_homogeneous_point* polygon, int point_count, float tx, float ty, float tz)
{
	//װ������ƽ�ƾ��� 
	float translate_matrix[4][4];
	memset(translate_matrix, 0, sizeof(int) * 16);
	translate_matrix[0][0] = translate_matrix[1][1] = translate_matrix[2][2] = translate_matrix[3][3] = 1;//�Խ��߸�ֵΪ1
	translate_matrix[0][3] = tx;
	translate_matrix[1][3] = ty;
	translate_matrix[2][3] = tz;

	//������ƽ�ƶ����ÿ������
	for (int vIndex = 0; vIndex < point_count; vIndex++)
	{
		struct my_homogeneous_point input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.z = polygon[vIndex].z;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v); //ƽ�ƾ������õ�ÿ������
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
		polygon[vIndex].z = input_v.z;
		polygon[vIndex].ratio = input_v.ratio;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
 
//////////////////////////////////////////////////�жϵ������ε�λ��////////////////////////////////////////////////////////
relative_position determine_position_for_a_point_in_any_polygon(struct my_homogeneous_point* polygon, int point_count, struct my_homogeneous_point input_point)
{
	//step 1����ʱ��˳�����ɵ�Ͷ���ι��ɵ�����
	my_3Dvector* generated_vectors = new my_3Dvector[point_count];//���ڴ洢�������ɵ�����input_pointָ��ÿ������	
	for (int vIndex = 0; vIndex < point_count; vIndex++)
	{
		my_3Dvector temp_vector(input_point, polygon[vIndex]);
		generated_vectors[vIndex] = temp_vector;
	}

	//step 2����������ÿ���߶�Ӧ�ĽǶȣ���ÿ������������֮���γɵļнǣ����ܺͣ����з���
	//		//�˴��ٶ�ͶӰ�泯��Ϊ��0,0,1����������ʵ���������
	my_3Dvector face_normal(0, 0, 1);
	float sigma_total_angle = 0;
	for (int eindex = 0; eindex < point_count; eindex++)
	{
		//��������ÿ���߶�Ӧ�ĽǶȣ���ÿ������������֮���γɵļнǣ�������
		int start_vec_index = eindex % point_count;
		int end_vec_index = (eindex + 1) % point_count;
		float dot_val = generated_vectors[start_vec_index].dot_multiply(generated_vectors[end_vec_index]);
		float costheta = dot_val / (generated_vectors[start_vec_index].len * generated_vectors[end_vec_index].len);
		float theta = acosf(costheta)*180/3.1415926535;

		//�����������������γɽǶ�������ʱ�루+������˳ʱ�루-��-����ͶӰ�泯������ˣ�������0����ͬΪ��ʱ��ȡ��������ȡ��
		my_3Dvector cross_vector = generated_vectors[start_vec_index].cross_multiply(generated_vectors[end_vec_index]);
		float sign = cross_vector.dot_multiply(face_normal) > 0 ? 1 : -1;

		sigma_total_angle += sign * theta;
	}

	delete [] generated_vectors;
	//step 3�������ۻ���sigma_total_angle��ȷ���������ڶ���ε��ڻ�����
	//���ǵ��������������, fabs(x)<=1e-4,��Ϊx=0��1e-4��С�����6λ
	return fabsf(sigma_total_angle) > 1e-4 ? relative_position::inside_on : relative_position::outside;
}

///////////////////////////////////////////////////////////////////////////////////////////
//���̽����¼�
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	case 'W':
	{
		my_traslate_homogeneous(clipping_polygon, clipping_polygon_point_count, 0, 5, 0);//��Y���������ƶ�1����Ԫ		
		glutPostRedisplay();
		break;
	}
	case 's':
	case 'S':
	{
		my_traslate_homogeneous(clipping_polygon, clipping_polygon_point_count, 0, -1, 0);//��Y�Ḻ�����ƶ�1����Ԫ
		glutPostRedisplay();
		break;
	}
	case 'a':
	case 'A':
	{
		my_traslate_homogeneous(clipping_polygon, clipping_polygon_point_count, -1, 0, 0);//��X�Ḻ�����ƶ�1����Ԫ
		glutPostRedisplay();
		break;
	}
	case 'd':
	case 'D':
	{
		my_traslate_homogeneous(clipping_polygon, clipping_polygon_point_count, 1, 0, 0); //��X���������ƶ�1����Ԫ
		glutPostRedisplay();
		break;
	}
	//case 'n':
	//case 'N':
	//{
	//	my_traslate_homogeneous(clipping_polygon, clipping_polygon_point_count, 0, 0, 1); //��z���������ƶ�1����Ԫ
	//	glutPostRedisplay();
	//	break;
	//}
	//case 'f':
	//case 'F':
	//{
	//	my_traslate_homogeneous(clipping_polygon, clipping_polygon_point_count, 0, 0, -1); //��z�Ḻ�����ƶ�1����Ԫ
	//	glutPostRedisplay();
	//	break;
	//}
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
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width, 0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width,
			-nearplane_distance, farplane_distance); //������ӵ�
	else
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height * (GLfloat)nearplane_width / (GLfloat)nearplane_height, 0.5 * nearplane_height * (GLfloat)nearplane_width / (GLfloat)nearplane_height,
			-nearplane_distance, farplane_distance); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
}

//��������
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_coordinate(); //��������ϵ

	//���Ʋ��Ե�
	glColor3f(0, 0, 0);
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex3i((floor)(test_point.x + 0.5), (floor)(test_point.y + 0.5), (floor)(test_point.z + 0.5));
	glEnd();

	glColor3f(157.0 / 256, 195.0 / 256, 230.0 / 256);

	//����clipping_polygon,Ĭ�϶���֮��ͨ��ֱ�߶�����
	glBegin(GL_LINES); //GL_LINE_LOOP
	
		for (int vindex = 0; vindex < clipping_polygon_point_count; vindex++)
		{
			int start_p_index = vindex % clipping_polygon_point_count;
			int end_p_index = (vindex+1) % clipping_polygon_point_count;

			glVertex3i((floor)(clipping_polygon[start_p_index].x + 0.5), (floor)(clipping_polygon[start_p_index].y + 0.5), (floor)(clipping_polygon[start_p_index].z + 0.5));
			glVertex3i((floor)(clipping_polygon[end_p_index].x + 0.5), (floor)(clipping_polygon[end_p_index].y + 0.5), (floor)(clipping_polygon[end_p_index].z + 0.5));
		}
	 
	glEnd();
	glutSwapBuffers();

	//�жϵ���ü����ڵ�λ��
	relative_position pos = determine_position_for_a_point_in_any_polygon(clipping_polygon, clipping_polygon_point_count, test_point);
	switch (pos)
	{
	case inside_on:
		printf("point is inside or on a polygon edge\n");
		break;
	case outside:
		printf("point is outside a polygon edge\n");
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
	glutCreateWindow("ƽ�Ʊ任");

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}


