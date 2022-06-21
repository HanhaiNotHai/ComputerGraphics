#define GLUT_DISABLE_ATEXIT_HACK
#include <stdlib.h>
#include <glut.h>
#include <math.h>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

int nearplane_width = 800; //�Ӿ�����
int nearplane_height = 800; //�Ӿ���߶�
int nearplane_distance = 50; //�Ӿ����ƽ�����ӵ����
int farplane_distance = nearplane_distance + 300; //�Ӿ���Զƽ�����ӵ����

//�Զ��������ڶ���εĿռ�λ�ù�ϵ
enum class relative_position
{
	inside_on, //�ڲ�
	outside //�ⲿ
};

//��ά��������µĵ㣨������߽��㣩
struct my_homogeneous_point_EX
{
	double x{};
	double y{};
	double z = 0;
	double ratio{};
	enum relative_position pos = relative_position::inside_on;
};

//�Զ����������ݽṹ�����ڴ洢��������ཻ�Ľ���
struct IntersectionPoint
{
	my_homogeneous_point_EX p;
	int pointFlag{}; //����Ƿ�Ϊ����: 1Ϊ���� 0Ϊԭ�е�
	int index0{}, index1{}; //���ڲü�����ε�index0 ���߶��� �ڱ��ü�����ε�index1���߶���
	bool inFlag{}; //����Ϊfalse ������Ĭ��Ϊtrue
	int distance{}; //������������߶����ľ��룬�����ڽ��������ʱ��ʹ��
};

//pg��Ķ���(��������㼯�����Ʋü���Ļ�·)
class Pg
{
public:
	vector<my_homogeneous_point_EX> pts;//��������
	Pg(void) {};
	~Pg(void) {};
	void drawPg()
	{
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < pts.size(); i++)
			glVertex2i(pts[i].x, pts[i].y);
		glEnd();
	};//���ƻ�·
};

Pg clip_polygon, subject_polygon;
bool clip = false;//�ж��Ƿ�ü�

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


//���ü������
#define subject_points_count 3
my_homogeneous_point_EX subject_points[subject_points_count];
//�ü������
#define clip_points_count 4
struct my_homogeneous_point_EX clip_points[clip_points_count];
//��ʼ���ü�����κͱ��ü�����εĶ���
void init()
{
	subject_points[0].x = -200; subject_points[0].y = -200;
	subject_points[1].x =  200; subject_points[1].y = -100;
	subject_points[2].x =   10; subject_points[2].y =  300;

	for (int i = 0; i < subject_points_count; i++)
	{
		subject_polygon.pts.push_back(subject_points[i]);
	}

	clip_points[0].x =  -50; clip_points[0].y =  -50;
	clip_points[1].x =  50; clip_points[1].y =  -50;
	clip_points[2].x =    50; clip_points[2].y = 50;
	clip_points[3].x =  -50; clip_points[3].y =  50;
	/*clip_points[4].x =  100; clip_points[4].y =  -20;
	clip_points[5].x =  220; clip_points[5].y =  250;*/

	for (int i = 0; i < clip_points_count; i++)
	{
		clip_polygon.pts.push_back(clip_points[i]);
	}
}

//�ۼƽǶȷ��жϵ��Ƿ��ڶ�����ڲ�
bool isPointInside(my_homogeneous_point_EX p, Pg& clip_polygon)
{
	double degree = 0;
	for (int i = 0; i < clip_polygon.pts.size(); i++)
	{
		my_homogeneous_point_EX p1 = clip_polygon.pts[i];
		my_homogeneous_point_EX p2 = clip_polygon.pts[(i + 1) % clip_polygon.pts.size()];
		double ab = (p1.x - p.x) * (p2.x - p.x) + (p1.y - p.y) * (p2.y - p.y);
		double a = sqrt((p1.x - p.x) * (p1.x - p.x) + (p1.y - p.y) * (p1.y - p.y));
		double b = sqrt((p2.x - p.x) * (p2.x - p.x) + (p2.y - p.y) * (p2.y - p.y));
		if ((p1.x - p.x) * (p2.y - p.y) - (p2.x - p.x) * (p1.y - p.y) >= 0)
		{
			degree += acos(ab / (a * b));
		}
		else
		{
			degree -= acos(ab / (a * b));
		}
	}
	int k = (int)(degree / (2 * 3.14));
	return (k % 2 == 1);
}

//��� ֱ��p0p1��p0p2
int cross(my_homogeneous_point_EX& p0, my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2)
{
	return ((p2.x - p0.x) * (p1.y - p0.y) - (p1.x - p0.x) * (p2.y - p0.y));
}

//�жϵ�p2�Ƿ����߶�p0,p1֮�� p0,p1,p2��ͬһֱ����
bool onSegment(my_homogeneous_point_EX& p0, my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2)
{
	int minx = min(p0.x, p1.x), maxx = max(p0.x, p1.x);
	int miny = min(p0.y, p1.y), maxy = max(p0.y, p1.y);
	if (p2.x >= minx && p2.x <= maxx && p2.y >= miny && p2.y <= maxy) return true;
	if ((p1.y - p0.y) / (p1.x - p0.x) == (p2.y - p0.y) / (p2.x - p0.x))return true;
	return false;
}

//�ж�����ֱ���Ƿ��н���
bool segmentsIntersect(my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2, my_homogeneous_point_EX& p3, my_homogeneous_point_EX& p4)
{
	int d1 = cross(p3, p4, p1);
	int d2 = cross(p3, p4, p2);
	int d3 = cross(p1, p2, p3);
	int d4 = cross(p1, p2, p4);
	if ( ( (d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0) ) && ( (d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0) ) )
		return true;
	if (d1 == 0 && onSegment(p3, p4, p1)) return true;
	if (d2 == 0 && onSegment(p3, p4, p2)) return true;
	if (d3 == 0 && onSegment(p1, p2, p3)) return true;
	if (d4 == 0 && onSegment(p1, p2, p4)) return true;
	return false;
}

//������������
my_homogeneous_point_EX getintersectPoint(my_homogeneous_point_EX p1, my_homogeneous_point_EX p2, my_homogeneous_point_EX p3, my_homogeneous_point_EX p4)
{
	my_homogeneous_point_EX p;
	int b1 = (p2.y - p1.y) * p1.x + (p1.x - p2.x) * p1.y;
	int b2 = (p4.y - p3.y) * p3.x + (p3.x - p4.x) * p3.y;
	int D = (p2.x - p1.x) * (p4.y - p3.y) - (p4.x - p3.x) * (p2.y - p1.y);
	int D1 = b2 * (p2.x - p1.x) - b1 * (p4.x - p3.x);
	int D2 = b2 * (p2.y - p1.y) - b1 * (p4.y - p3.y);
	p.x = D1 / D;
	p.y = D2 / D;
	return p;
}

//���������������н���
void generateIntersectPoints(Pg& clip_polygon, Pg& subject_polygon, list<IntersectionPoint>& plist)
{
	int clip_polygonSize = clip_polygon.pts.size(), subject_polygonSize = subject_polygon.pts.size();
	for (int i = 0; i < clip_polygonSize; i++)
	{
		my_homogeneous_point_EX p1 = clip_polygon.pts[i];
		my_homogeneous_point_EX p2 = clip_polygon.pts[(i + 1) % clip_polygonSize];
		for (int j = 0; j < subject_polygonSize; j++)
		{
			my_homogeneous_point_EX p3 = subject_polygon.pts[j];
			my_homogeneous_point_EX p4 = subject_polygon.pts[(j + 1) % subject_polygonSize];
			//�ж����߶��Ƿ��н���
			if (segmentsIntersect(p1, p2, p3, p4))
			{
				IntersectionPoint ip;
				ip.index0 = i;//���ڲü�����ε�i��ֱ����
				ip.index1 = j;//���ڱ��ü�����ε�j��ֱ����
				ip.p = getintersectPoint(p1, p2, p3, p4);//���߶εĽ���
				plist.push_back(ip);
			}
		}
	}
}

//��p1 p2��ľ��� ƽ��
int getDistance(my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

//����Ƚ�
bool distanceComparator(IntersectionPoint& ip1, IntersectionPoint& ip2)
{
	return ip1.distance < ip2.distance;
}

//��plist�еĵ㰴�����comlist�� indexΪ0��ʾ�ü�����Σ�indexΪ1��ʾ���ü������
void generateList(Pg& com, list<IntersectionPoint>& plist, list<IntersectionPoint>& comlist, int index)
{
	int size = com.pts.size();
	list<IntersectionPoint>::iterator it;

	for (int i = 0; i < size; i++)
	{
		my_homogeneous_point_EX p1 = com.pts[i];
		IntersectionPoint ip;
		ip.pointFlag = 0;
		ip.p = p1;
		comlist.push_back(ip);
		list<IntersectionPoint> oneSeg;
		for (it = plist.begin(); it != plist.end(); it++)
		{
			if ((index == 0 && i == it->index0) || (index == 1 && i == it->index1))
			{
				it->distance = getDistance(it->p, p1);
				it->pointFlag = 1;
				oneSeg.push_back(*it);
			}
		}
		//������ĵ㰴�վ�������
		oneSeg.sort(distanceComparator);
		for (it = oneSeg.begin(); it != oneSeg.end(); it++)
			comlist.push_back(*it);
	}
}

//�жϵ�����㻹�ǳ��� ����Ϊfalse ������Ĭ��Ϊtrue
void getsubject_polygonPointInOut(Pg& clip_polygon, list<IntersectionPoint>& subject_polygonlist)
{
	bool inFlag = true;
	list<IntersectionPoint>::iterator it;
	for (it = subject_polygonlist.begin(); it != subject_polygonlist.end(); it++)
	{
		if (it->pointFlag == 0) //�ǽ�������
		{
			inFlag = isPointInside(it->p, clip_polygon);
		}
		else //��������
		{
			inFlag = !inFlag;
			it->inFlag = inFlag;
		}
	}
}

//�ж�p1 p2 �Ƿ�Ϊͬһ�� �������
bool operator==(my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2)
{
	return p1.x == p2.x && p1.y == p2.y;
}

//�ж�������β���Ƿ����
bool circle(Pg point, list<IntersectionPoint>::iterator it1)
{
	if (point.pts[0] == it1->p)
	{
		return true;
	}
	return false;//��β�����
}

//�ж�������
void getclip_polygonPointInOut(list<IntersectionPoint>& clip_polygonlist, list<IntersectionPoint>& Pglist)
{
	list<IntersectionPoint>::iterator it1, it2;
	for (it1 = clip_polygonlist.begin(); it1 != clip_polygonlist.end(); it1++)
	{
		if (it1->pointFlag == 0) continue;
		for (it2 = Pglist.begin(); it2 != Pglist.end(); it2++)
		{
			if (it2->pointFlag == 0)
				continue;
			// ��Ϊ�����ڲü�����κͱ��ü�����ζ����� ����ͨ��ƥ��ĸ�clip_polygonlist��Ľ��㸳��㡢�����ֵ
			if (it1->p == it2->p)
				it1->inFlag = it2->inFlag;
		}
	}
}

void generateClipAreas(list<IntersectionPoint>& clip_polygonlist, list<IntersectionPoint>& subject_polygonlist, list<IntersectionPoint>& plist)
{
	list<IntersectionPoint>::iterator it1, it2;
	list<IntersectionPoint>::iterator it3; //it3�����ȽϽ����Ƿ񶼱�����
	Pg subject_polygonClip; //��ʱ�ü����
	Pg allpoints; //������¼���б������ĵ�
	bool visitall = false;

	//�ڱ��ü������subject_polygonlist���ҵ��ǽ�����Ϊ���ĵ�һ����
	for (it2 = subject_polygonlist.begin(); it2 != subject_polygonlist.end(); it2++)
		if (it2->pointFlag == 1 && it2->inFlag) break;

	subject_polygonClip.pts.clear();
	while (visitall == false) //��plist�����н��㱻���ʣ�������ѭ��
	{
		visitall = true;
		for (it3 = plist.begin(); it3 != plist.end(); it3++)
		{
			bool flag = false;//����plist�е�ÿһ����۲��Ƿ񱻲ü�
			if (allpoints.pts.size() == 0)
			{
				visitall = false;
			}
			for (int i = 0; i < allpoints.pts.size(); i++)
			{
				//����ҵ���Ӧ��
				if ((it3->p.x == allpoints.pts[i].x) && (it3->p.y == allpoints.pts[i].y))
				{
					flag = true;
				}

				if (flag == false)
				{
					if (i == allpoints.pts.size() - 1) //���������û�ҵ���Ӧ��
					{
						visitall = false;
					}
				}
			}
		}

		//ȫ�������ʣ�ֱ������ѭ��
		if (visitall == true)
		{
			continue;
		}

		//ѭ������subject_polygonlist 
		it2 = (it2 == subject_polygonlist.end()) ? subject_polygonlist.begin() : it2;
		subject_polygonClip.pts.push_back(it2->p);
		allpoints.pts.push_back(it2->p);
		it2++;
		it2 = (it2 == subject_polygonlist.end()) ? subject_polygonlist.begin() : it2;
		
		for (; it2 != subject_polygonlist.end(); it2++)
		{
			//subject_polygonlist��������β��Ҫ��ͷ��ʼ
			if (it2->pointFlag == 1 && !it2->inFlag) break;
			subject_polygonClip.pts.push_back(it2->p);
			allpoints.pts.push_back(it2->p);
		}
		
		//��clip_polygonlist����ͬһ��
		for (it1 = clip_polygonlist.begin(); it1 != clip_polygonlist.end(); it1++)
			if (it1->p == it2->p) break;
		
		for (; it1 != clip_polygonlist.end(); it1++)
		{
			if (it1->pointFlag == 1 && it1->inFlag) break;
			subject_polygonClip.pts.push_back(it1->p);
			allpoints.pts.push_back(it1->p);
		}
		
		//��β����ȣ��ҵ�һ���ü����������������
		if (circle(subject_polygonClip, it1))
		{
			//���subject_polygonClip;
			subject_polygonClip.drawPg();
			subject_polygonClip.pts.clear();
			for (; it2 != subject_polygonlist.end(); it2++) //δ��subject_polygonlist��β�������ڱ��ü������subject_polygonlist���ҵ��ǽ��� ��Ϊ���ĵ�һ����
				if (it2->pointFlag == 1 && it2->inFlag) break;
			continue;//�ص������while���ж�while (visitall==false)
		}
		
		//��subject_polygonlist����ͬһ��
		for (; it2 != subject_polygonlist.end(); it2++)
			if (it2->p == it1->p) break;
	}
}

//�ü�����
void weilerAtherton(Pg& clip_polygon, Pg& subject_polygon)
{
	list<IntersectionPoint> plist, subject_polygonlist, clip_polygonlist;
	generateIntersectPoints(clip_polygon, subject_polygon, plist);//������н������plist��
	
	//�����㰴��������εĶ���������
	generateList(clip_polygon, plist, clip_polygonlist, 0);
	generateList(subject_polygon, plist, subject_polygonlist, 1);
	
	//�ж�ÿ�����������clip_polygon�ǳ��㻹�����
	getsubject_polygonPointInOut(clip_polygon, subject_polygonlist);//�ü������ ���뽻��ı��ü����������
	getclip_polygonPointInOut(clip_polygonlist, subject_polygonlist);
	
	//���ɲü�����
	generateClipAreas(clip_polygonlist, subject_polygonlist, plist);
}

//���̽����¼�
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case'1':
	{
		clip = true;
		glutPostRedisplay();
		break;
	}
	case'2':
	{
		clip = false;
		glutPostRedisplay();
		break;
	}
	case 'w':
	case 'W':
	{
		clip_polygon.pts.clear();
		for (int i = 0; i < clip_points_count; i++)
		{
			clip_points[i].y += 11;
			clip_polygon.pts.push_back(clip_points[i]);
		}
		glutPostRedisplay();
		break;
	}
	case's':
	{
		clip_polygon.pts.clear();
		for (int i = 0; i < clip_points_count; i++)
		{
			clip_points[i].y -= 11;
			clip_polygon.pts.push_back(clip_points[i]);
		}
		glutPostRedisplay();
		break;
	}
	case'a':
	{
		clip_polygon.pts.clear();
		for (int i = 0; i < clip_points_count; i++)
		{
			clip_points[i].x -= 11;
			clip_polygon.pts.push_back(clip_points[i]);
		}
		glutPostRedisplay();
		break;
	}
	case'd':
	{
		clip_polygon.pts.clear();
		for (int i = 0; i < clip_points_count; i++)
		{
			clip_points[i].x += 11;
			clip_polygon.pts.push_back(clip_points[i]);
		}
		glutPostRedisplay();
		break;
	}
	case 27:
	{
		exit(0);
		break;
	}
	default:
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
}

//��������
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLineWidth(2);
	draw_coordinate();

	//����ͼ��
	glColor3f(0.0f, 0.0f, 0.0f);
	clip_polygon.drawPg();

	glColor3f(0.0f, 0.0f, 1.0f);
	if (true == clip)
	{
		weilerAtherton(clip_polygon, subject_polygon);
	}
	else
	{
		subject_polygon.drawPg();
	}

	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(nearplane_width, nearplane_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Weiler-Atherton����βü�");

	init();
	
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}