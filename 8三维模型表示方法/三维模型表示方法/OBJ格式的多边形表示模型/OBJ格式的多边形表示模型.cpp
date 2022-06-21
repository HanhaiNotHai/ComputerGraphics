//9.2OBJ 格式的多边形表示模型.cpp
#define GLUT_DISABLE_ATEXIT_HACK
#include "ObjLoader.h"
#include "GLUT.H"

int nearplane_width = 600; //视景体宽度
int nearplane_height = 600; //视景体高度
int nearplane_distance = 500; //视景体近平面与视点距离
int farplane_distance = nearplane_distance + 300; //视景体远平面与视点距离

float eye_x = 20;
float eye_y = 20;
float eye_z = 20;
float theta = 0.1;

my_triangle_3DModel cur3DModel; //场景中的3D模型

//模型加载
void init(void)
{
	ObjLoader objModel = ObjLoader("lily-impeller.obj");
	//ObjLoader objModel = ObjLoader("../../测试2.obj");
	cur3DModel = objModel.my_3DModel;
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

	glColor3f(0.0, 0.0, 1.0);//蓝色z轴
	glVertex3f(0.0, 0.0, nearplane_height);
	glVertex3f(0.0, 0.0, -nearplane_height);

	glEnd();
}

//绘制内容
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_coordinate();

	glColor3f(157.0 / 256, 195.0 / 256, 230.0 / 256);
	glColor3f(0, 0, 0);

	//开始绘制
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < cur3DModel.faceSets.size(); i++)
	{
		//取出顶点序号获得相应顶点坐标
		my_3D_point_coord point1, point2, point3;
		int firstPointIndex = cur3DModel.faceSets[i].first_point_index;
		int secondPointIndex = cur3DModel.faceSets[i].second_point_index;
		int thirdPointIndex = cur3DModel.faceSets[i].third_point_index;

		point1 = cur3DModel.pointSets[firstPointIndex]; //第一个顶点
		point2 = cur3DModel.pointSets[secondPointIndex]; //第二个顶点
		point3 = cur3DModel.pointSets[thirdPointIndex]; //第三个顶点

		//取出法向序号获得相应法向
		my_3Dvector vector1, vector2, vector3;
		int firstNormalIndex = cur3DModel.faceSets[i].first_point_normal_index;
		int secondNormalIndex = cur3DModel.faceSets[i].second_point_normal_index;
		int thirdNormalIndex = cur3DModel.faceSets[i].third_point_normal_index;

		vector1 = cur3DModel.pointNormalSets[firstNormalIndex]; //第一个点的法向量
		vector2 = cur3DModel.pointNormalSets[secondNormalIndex]; //第二个点的法向量
		vector3 = cur3DModel.pointNormalSets[thirdNormalIndex]; //第三个点的法向量

		//取出纹理坐标序号获得相应纹理坐标
		my_2D_Texture_coord texture1, texture2, texture3;
		int firstTextureIndex = cur3DModel.faceSets[i].first_point_texture_index;
		int secondTextureIndex = cur3DModel.faceSets[i].second_point_texture_index;
		int thirdTextureIndex = cur3DModel.faceSets[i].third_point_texture_index;

		texture1 = cur3DModel.pointTextureSets[firstTextureIndex]; //第一个点的纹理
		texture2 = cur3DModel.pointTextureSets[secondTextureIndex]; //第二个点的纹理
		texture3 = cur3DModel.pointTextureSets[thirdTextureIndex]; //第三个点的纹理

		//绘制三角网格面
		glNormal3f(vector1.dx, vector1.dy, vector1.dz); //绑定顶点的法向
		glTexCoord2f(texture1.u, texture1.v); //绑定顶点的纹理
		glVertex3f(point1.x, point1.y, point1.z); //绘制顶点

		glNormal3f(vector2.dx, vector2.dy, vector2.dz);
		glTexCoord2f(texture2.u, texture2.v);
		glVertex3f(point2.x, point2.y, point2.z);

		glNormal3f(vector3.dx, vector3.dy, vector3.dz);
		glTexCoord2f(texture3.u, texture3.v);
		glVertex3f(point3.x, point3.y, point3.z);
	}
	glEnd();

	glutSwapBuffers();
}

//投影方式、modelview方式设置
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height *
			(GLfloat)nearplane_height / (GLfloat)nearplane_width, 0.5 * nearplane_height *
			(GLfloat)nearplane_height / (GLfloat)nearplane_width,
			-nearplane_distance, farplane_distance); //相对于视点
	else
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height *
			(GLfloat)nearplane_width / (GLfloat)nearplane_height, 0.5 * nearplane_height *
			(GLfloat)nearplane_width / (GLfloat)nearplane_height,
			-nearplane_distance, farplane_distance);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_x, eye_y, eye_z, 0, 0, 0, 0, 1, 0);
}

//键盘交互事件
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case'a':
	{
		eye_x = eye_x * cosf(-theta) + eye_z * sinf(-theta);
		eye_z = eye_z * cosf(-theta) - eye_x * sinf(-theta);
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case'd':
	{
		eye_x = eye_x * cosf(theta) + eye_z * sinf(theta);
		eye_z = eye_z * cosf(theta) - eye_x * sinf(theta);
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case'w':
	{
		eye_y = eye_y * cosf(theta) + eye_z * sinf(theta);
		eye_z = eye_z * cosf(theta) - eye_y * sinf(theta);
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case's':
	{
		eye_y = eye_y * cosf(-theta) + eye_z * sinf(-theta);
		eye_z = eye_z * cosf(-theta) - eye_y * sinf(-theta);
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case'q':
	{
		eye_x = eye_x * cosf(-theta) + eye_y * sinf(-theta);
		eye_y = eye_y * cosf(-theta) - eye_x * sinf(-theta);
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case'e':
	{
		eye_x = eye_x * cosf(theta) + eye_y * sinf(theta);
		eye_y = eye_y * cosf(theta) - eye_x * sinf(theta);
		reshape(nearplane_width, nearplane_height);
		glutPostRedisplay();
		break;
	}
	case 'z': //打开深度缓冲测试
	case 'Z':
	{
		glEnable(GL_DEPTH_TEST); //打开深度缓冲测试
		glDepthFunc(GL_LEQUAL); //判断遮挡关系时，离视点近的物体遮挡离视点远的物体
		glutPostRedisplay();
		break;
	}
	case 'c': //关闭深度缓冲测试
	case 'C':
	{
		glDisable(GL_DEPTH_TEST); //关闭深度缓冲测试
		glutPostRedisplay();
		break;
	}
	case 'l': //打开灯光
	case 'L':
	{
		GLfloat light_position[] = { 100.0, 100.0, 100.0, 0.0 };
		GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		GLfloat diffuse[] = { 1, 1, 1, 1.0 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);

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
			eye_x = eye_x * cosf(-theta) + eye_z * sinf(-theta);
			eye_z = eye_z * cosf(-theta) - eye_x * sinf(-theta);
			reshape(nearplane_width, nearplane_height);
			glShadeModel(GL_FLAT);
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
		{
			eye_x = eye_x * cosf(theta) + eye_z * sinf(theta);
			eye_z = eye_z * cosf(theta) - eye_x * sinf(theta);
			reshape(nearplane_width, nearplane_height);
			glutPostRedisplay();
		}
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
	glutInitWindowPosition(400, 300);
	glutCreateWindow("三维模型加载");

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}