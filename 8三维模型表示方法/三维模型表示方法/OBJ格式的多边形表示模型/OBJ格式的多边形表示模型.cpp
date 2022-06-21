//9.2OBJ ��ʽ�Ķ���α�ʾģ��.cpp
#define GLUT_DISABLE_ATEXIT_HACK
#include "ObjLoader.h"
#include "GLUT.H"

int nearplane_width = 600; //�Ӿ�����
int nearplane_height = 600; //�Ӿ���߶�
int nearplane_distance = 500; //�Ӿ����ƽ�����ӵ����
int farplane_distance = nearplane_distance + 300; //�Ӿ���Զƽ�����ӵ����

float eye_x = 20;
float eye_y = 20;
float eye_z = 20;
float theta = 0.1;

my_triangle_3DModel cur3DModel; //�����е�3Dģ��

//ģ�ͼ���
void init(void)
{
	ObjLoader objModel = ObjLoader("lily-impeller.obj");
	//ObjLoader objModel = ObjLoader("../../����2.obj");
	cur3DModel = objModel.my_3DModel;
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

//��������
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	draw_coordinate();

	glColor3f(157.0 / 256, 195.0 / 256, 230.0 / 256);
	glColor3f(0, 0, 0);

	//��ʼ����
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < cur3DModel.faceSets.size(); i++)
	{
		//ȡ��������Ż����Ӧ��������
		my_3D_point_coord point1, point2, point3;
		int firstPointIndex = cur3DModel.faceSets[i].first_point_index;
		int secondPointIndex = cur3DModel.faceSets[i].second_point_index;
		int thirdPointIndex = cur3DModel.faceSets[i].third_point_index;

		point1 = cur3DModel.pointSets[firstPointIndex]; //��һ������
		point2 = cur3DModel.pointSets[secondPointIndex]; //�ڶ�������
		point3 = cur3DModel.pointSets[thirdPointIndex]; //����������

		//ȡ��������Ż����Ӧ����
		my_3Dvector vector1, vector2, vector3;
		int firstNormalIndex = cur3DModel.faceSets[i].first_point_normal_index;
		int secondNormalIndex = cur3DModel.faceSets[i].second_point_normal_index;
		int thirdNormalIndex = cur3DModel.faceSets[i].third_point_normal_index;

		vector1 = cur3DModel.pointNormalSets[firstNormalIndex]; //��һ����ķ�����
		vector2 = cur3DModel.pointNormalSets[secondNormalIndex]; //�ڶ�����ķ�����
		vector3 = cur3DModel.pointNormalSets[thirdNormalIndex]; //��������ķ�����

		//ȡ������������Ż����Ӧ��������
		my_2D_Texture_coord texture1, texture2, texture3;
		int firstTextureIndex = cur3DModel.faceSets[i].first_point_texture_index;
		int secondTextureIndex = cur3DModel.faceSets[i].second_point_texture_index;
		int thirdTextureIndex = cur3DModel.faceSets[i].third_point_texture_index;

		texture1 = cur3DModel.pointTextureSets[firstTextureIndex]; //��һ���������
		texture2 = cur3DModel.pointTextureSets[secondTextureIndex]; //�ڶ����������
		texture3 = cur3DModel.pointTextureSets[thirdTextureIndex]; //�������������

		//��������������
		glNormal3f(vector1.dx, vector1.dy, vector1.dz); //�󶨶���ķ���
		glTexCoord2f(texture1.u, texture1.v); //�󶨶��������
		glVertex3f(point1.x, point1.y, point1.z); //���ƶ���

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

//ͶӰ��ʽ��modelview��ʽ����
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (w <= h)
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height *
			(GLfloat)nearplane_height / (GLfloat)nearplane_width, 0.5 * nearplane_height *
			(GLfloat)nearplane_height / (GLfloat)nearplane_width,
			-nearplane_distance, farplane_distance); //������ӵ�
	else
		glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height *
			(GLfloat)nearplane_width / (GLfloat)nearplane_height, 0.5 * nearplane_height *
			(GLfloat)nearplane_width / (GLfloat)nearplane_height,
			-nearplane_distance, farplane_distance);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye_x, eye_y, eye_z, 0, 0, 0, 0, 1, 0);
}

//���̽����¼�
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
	case 'z': //����Ȼ������
	case 'Z':
	{
		glEnable(GL_DEPTH_TEST); //����Ȼ������
		glDepthFunc(GL_LEQUAL); //�ж��ڵ���ϵʱ�����ӵ���������ڵ����ӵ�Զ������
		glutPostRedisplay();
		break;
	}
	case 'c': //�ر���Ȼ������
	case 'C':
	{
		glDisable(GL_DEPTH_TEST); //�ر���Ȼ������
		glutPostRedisplay();
		break;
	}
	case 'l': //�򿪵ƹ�
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

//��꽻���¼�
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

//��������
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(nearplane_width, nearplane_height);
	glutInitWindowPosition(400, 300);
	glutCreateWindow("��άģ�ͼ���");

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();
	return 0;
}