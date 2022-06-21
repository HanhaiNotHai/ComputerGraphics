#define GLUT_DISABLE_ATEXIT_HACK
#include "GLUT.H"
#include <math.h>
#include <string.h> 
#include <stdio.h>
#include "My_3DVector.h"

int nearplane_width = 600; //视景体宽度
int nearplane_height = 600; //视景体高度
int nearplane_distance = 50; //视景体近平面与视点距离
int farplane_distance = nearplane_distance + 300; //视景体远平面与视点距离
   
//测试顶点
my_homogeneous_point test_point;

//裁剪多边形顶点坐标
//每条边都是直线
#define clipping_polygon_point_count 4
struct my_homogeneous_point clipping_polygon[clipping_polygon_point_count];

//被裁剪多边形顶点坐标
//每条边都是直线
struct my_homogeneous_point subject_polygon[4];

//初始化正方形顶点坐标
void init(void)
{
	test_point.x = 50;
	test_point.y = 50;
	test_point.z = 0;

	//前面四个顶点
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

//绘制坐标系
void draw_coordinate()
{
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0); //红色x轴
		glVertex3f(nearplane_width, 0.0, 0.0);
		glVertex3f(-nearplane_width, 0.0, 0.0);

		glColor3f(0.0, 1.0, 0.0);//绿色y轴
		glVertex3f(0.0, nearplane_height, 0.0);
		glVertex3f(0.0, -nearplane_height, 0.0); 	 
	glEnd();
}

//////////////////////////////////////////////齐次平移变换-所需函数//////////////////////////////////////////////
//矩阵向量相乘
struct my_homogeneous_point matrix_multiply_vector(float matrix[][4], struct my_homogeneous_point input_v)
{
	struct my_homogeneous_point translated_v;
	translated_v.x = matrix[0][0] * input_v.x + matrix[0][1] * input_v.y + matrix[0][2] * input_v.z + matrix[0][3] * input_v.ratio;
	translated_v.y = matrix[1][0] * input_v.x + matrix[1][1] * input_v.y + matrix[1][2] * input_v.z + matrix[1][3] * input_v.ratio;
	translated_v.z = matrix[2][0] * input_v.x + matrix[2][1] * input_v.y + matrix[2][2] * input_v.z + matrix[2][3] * input_v.ratio;
	translated_v.ratio = matrix[3][0] * input_v.x + matrix[3][1] * input_v.y + matrix[3][2] * input_v.z + matrix[3][3] * input_v.ratio;
	return translated_v;
}

//齐次平移变换
void my_traslate_homogeneous(struct my_homogeneous_point* polygon, int point_count, float tx, float ty, float tz)
{
	//装配生成平移矩阵 
	float translate_matrix[4][4];
	memset(translate_matrix, 0, sizeof(int) * 16);
	translate_matrix[0][0] = translate_matrix[1][1] = translate_matrix[2][2] = translate_matrix[3][3] = 1;//对角线赋值为1
	translate_matrix[0][3] = tx;
	translate_matrix[1][3] = ty;
	translate_matrix[2][3] = tz;

	//遍历并平移多边形每个顶点
	for (int vIndex = 0; vIndex < point_count; vIndex++)
	{
		struct my_homogeneous_point input_v;
		input_v.x = polygon[vIndex].x;
		input_v.y = polygon[vIndex].y;
		input_v.z = polygon[vIndex].z;
		input_v.ratio = 1;
		input_v = matrix_multiply_vector(translate_matrix, input_v); //平移矩阵作用到每个顶点
		polygon[vIndex].x = input_v.x;
		polygon[vIndex].y = input_v.y;
		polygon[vIndex].z = input_v.z;
		polygon[vIndex].ratio = input_v.ratio;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////
 
//////////////////////////////////////////////////判断点与多边形的位置////////////////////////////////////////////////////////
relative_position determine_position_for_a_point_in_any_polygon(struct my_homogeneous_point* polygon, int point_count, struct my_homogeneous_point input_point)
{
	//step 1：逆时针顺序生成点和多边形构成的向量
	my_3Dvector* generated_vectors = new my_3Dvector[point_count];//用于存储所有生成的向量input_point指向每个顶点	
	for (int vIndex = 0; vIndex < point_count; vIndex++)
	{
		my_3Dvector temp_vector(input_point, polygon[vIndex]);
		generated_vectors[vIndex] = temp_vector;
	}

	//step 2：计算多边形每条边对应的角度，即每两条相邻向量之间形成的夹角，的总和（带有方向）
	//		//此处假定投影面朝向为（0,0,1）——根据实际情况设置
	my_3Dvector face_normal(0, 0, 1);
	float sigma_total_angle = 0;
	for (int eindex = 0; eindex < point_count; eindex++)
	{
		//计算多边形每条边对应的角度，即每两条相邻向量之间形成的夹角，无正负
		int start_vec_index = eindex % point_count;
		int end_vec_index = (eindex + 1) % point_count;
		float dot_val = generated_vectors[start_vec_index].dot_multiply(generated_vectors[end_vec_index]);
		float costheta = dot_val / (generated_vectors[start_vec_index].len * generated_vectors[end_vec_index].len);
		float theta = acosf(costheta)*180/3.1415926535;

		//计算相邻两条向量形成角度属于逆时针（+）还是顺时针（-）-》与投影面朝向做点乘，若大于0，则同为逆时针取﹢，否则取﹣
		my_3Dvector cross_vector = generated_vectors[start_vec_index].cross_multiply(generated_vectors[end_vec_index]);
		float sign = cross_vector.dot_multiply(face_normal) > 0 ? 1 : -1;

		sigma_total_angle += sign * theta;
	}

	delete [] generated_vectors;
	//step 3：根据累积角sigma_total_angle，确定给定点在多边形的内还是外
	//考虑到浮点数舍入误差, fabs(x)<=1e-4,认为x=0；1e-4即小数点后6位
	return fabsf(sigma_total_angle) > 1e-4 ? relative_position::inside_on : relative_position::outside;
}

///////////////////////////////////////////////////////////////////////////////////////////
//键盘交互事件
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
	case 'W':
	{
		my_traslate_homogeneous(clipping_polygon, clipping_polygon_point_count, 0, 5, 0);//向Y轴正方向移动1个单元		
		glutPostRedisplay();
		break;
	}
	case 's':
	case 'S':
	{
		my_traslate_homogeneous(clipping_polygon, clipping_polygon_point_count, 0, -1, 0);//向Y轴负方向移动1个单元
		glutPostRedisplay();
		break;
	}
	case 'a':
	case 'A':
	{
		my_traslate_homogeneous(clipping_polygon, clipping_polygon_point_count, -1, 0, 0);//向X轴负方向移动1个单元
		glutPostRedisplay();
		break;
	}
	case 'd':
	case 'D':
	{
		my_traslate_homogeneous(clipping_polygon, clipping_polygon_point_count, 1, 0, 0); //向X轴正方向移动1个单元
		glutPostRedisplay();
		break;
	}
	//case 'n':
	//case 'N':
	//{
	//	my_traslate_homogeneous(clipping_polygon, clipping_polygon_point_count, 0, 0, 1); //向z轴正方向移动1个单元
	//	glutPostRedisplay();
	//	break;
	//}
	//case 'f':
	//case 'F':
	//{
	//	my_traslate_homogeneous(clipping_polygon, clipping_polygon_point_count, 0, 0, -1); //向z轴负方向移动1个单元
	//	glutPostRedisplay();
	//	break;
	//}
	case 27:
		exit(0);
		break;
	}
} 

//投影方式、modelview方式设置
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width, 0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width,
			-nearplane_distance, farplane_distance); //相对于视点
	else
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height * (GLfloat)nearplane_width / (GLfloat)nearplane_height, 0.5 * nearplane_height * (GLfloat)nearplane_width / (GLfloat)nearplane_height,
			-nearplane_distance, farplane_distance); 

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
}

//绘制内容
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	draw_coordinate(); //绘制坐标系

	//绘制测试点
	glColor3f(0, 0, 0);
	glPointSize(5);
	glBegin(GL_POINTS);
	glVertex3i((floor)(test_point.x + 0.5), (floor)(test_point.y + 0.5), (floor)(test_point.z + 0.5));
	glEnd();

	glColor3f(157.0 / 256, 195.0 / 256, 230.0 / 256);

	//绘制clipping_polygon,默认顶点之间通过直线段相连
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

	//判断点与裁剪窗口的位置
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

//主调函数
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(nearplane_width, nearplane_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("平移变换");

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}


