#define GLUT_DISABLE_ATEXIT_HACK
#include <stdlib.h>
#include <glut.h>
#include <math.h>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

int nearplane_width = 800; //视景体宽度
int nearplane_height = 800; //视景体高度
int nearplane_distance = 50; //视景体近平面与视点距离
int farplane_distance = nearplane_distance + 300; //视景体远平面与视点距离

//自定义点相对于多边形的空间位置关系
enum class relative_position
{
	inside_on, //内部
	outside //外部
};

//三维齐次坐标下的点（顶点或者交点）
struct my_homogeneous_point_EX
{
	double x{};
	double y{};
	double z = 0;
	double ratio{};
	enum relative_position pos = relative_position::inside_on;
};

//自定义以下数据结构，用于存储两多边形相交的交点
struct IntersectionPoint
{
	my_homogeneous_point_EX p;
	int pointFlag{}; //标记是否为交点: 1为交点 0为原有点
	int index0{}, index1{}; //点在裁剪多边形第index0 条线段上 在被裁减多边形第index1条线段上
	bool inFlag{}; //出点为false 其他点默认为true
	int distance{}; //交点距离所在线段起点的距离，用于在交点排序的时候使用
};

//pg类的定义(用来储存点集，绘制裁剪后的回路)
class Pg
{
public:
	vector<my_homogeneous_point_EX> pts;//顶点数组
	Pg(void) {};
	~Pg(void) {};
	void drawPg()
	{
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < pts.size(); i++)
			glVertex2i(pts[i].x, pts[i].y);
		glEnd();
	};//绘制回路
};

Pg clip_polygon, subject_polygon;
bool clip = false;//判断是否裁剪

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


//被裁剪多边形
#define subject_points_count 3
my_homogeneous_point_EX subject_points[subject_points_count];
//裁剪多边形
#define clip_points_count 4
struct my_homogeneous_point_EX clip_points[clip_points_count];
//初始化裁剪多边形和被裁剪多边形的顶点
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

//累计角度法判断点是否在多边形内部
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

//叉乘 直线p0p1和p0p2
int cross(my_homogeneous_point_EX& p0, my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2)
{
	return ((p2.x - p0.x) * (p1.y - p0.y) - (p1.x - p0.x) * (p2.y - p0.y));
}

//判断点p2是否在线段p0,p1之间 p0,p1,p2在同一直线上
bool onSegment(my_homogeneous_point_EX& p0, my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2)
{
	int minx = min(p0.x, p1.x), maxx = max(p0.x, p1.x);
	int miny = min(p0.y, p1.y), maxy = max(p0.y, p1.y);
	if (p2.x >= minx && p2.x <= maxx && p2.y >= miny && p2.y <= maxy) return true;
	if ((p1.y - p0.y) / (p1.x - p0.x) == (p2.y - p0.y) / (p2.x - p0.x))return true;
	return false;
}

//判断两条直线是否有交点
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

//求出交点的坐标
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

//求出两个多边形所有交点
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
			//判断两线段是否有交点
			if (segmentsIntersect(p1, p2, p3, p4))
			{
				IntersectionPoint ip;
				ip.index0 = i;//点在裁剪多边形第i条直线上
				ip.index1 = j;//点在被裁剪多边形第j条直线上
				ip.p = getintersectPoint(p1, p2, p3, p4);//两线段的交点
				plist.push_back(ip);
			}
		}
	}
}

//求p1 p2点的距离 平方
int getDistance(my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

//距离比较
bool distanceComparator(IntersectionPoint& ip1, IntersectionPoint& ip2)
{
	return ip1.distance < ip2.distance;
}

//将plist中的点按序插入comlist中 index为0表示裁剪多边形，index为1表示被裁剪多边形
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
		//将插入的点按照距离排序
		oneSeg.sort(distanceComparator);
		for (it = oneSeg.begin(); it != oneSeg.end(); it++)
			comlist.push_back(*it);
	}
}

//判断点是入点还是出点 出点为false 其他点默认为true
void getsubject_polygonPointInOut(Pg& clip_polygon, list<IntersectionPoint>& subject_polygonlist)
{
	bool inFlag = true;
	list<IntersectionPoint>::iterator it;
	for (it = subject_polygonlist.begin(); it != subject_polygonlist.end(); it++)
	{
		if (it->pointFlag == 0) //非交点的情况
		{
			inFlag = isPointInside(it->p, clip_polygon);
		}
		else //交点的情况
		{
			inFlag = !inFlag;
			it->inFlag = inFlag;
		}
	}
}

//判断p1 p2 是否为同一点 近似相等
bool operator==(my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2)
{
	return p1.x == p2.x && p1.y == p2.y;
}

//判断链表首尾点是否相等
bool circle(Pg point, list<IntersectionPoint>::iterator it1)
{
	if (point.pts[0] == it1->p)
	{
		return true;
	}
	return false;//首尾不相等
}

//判断入点出点
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
			// 因为交点在裁剪多边形和被裁减多边形都存在 可以通过匹配的给clip_polygonlist里的交点赋入点、出点的值
			if (it1->p == it2->p)
				it1->inFlag = it2->inFlag;
		}
	}
}

void generateClipAreas(list<IntersectionPoint>& clip_polygonlist, list<IntersectionPoint>& subject_polygonlist, list<IntersectionPoint>& plist)
{
	list<IntersectionPoint>::iterator it1, it2;
	list<IntersectionPoint>::iterator it3; //it3用来比较交点是否都被遍历
	Pg subject_polygonClip; //临时裁剪结果
	Pg allpoints; //用来记录所有遍历过的点
	bool visitall = false;

	//在被裁减多边形subject_polygonlist中找到是交点且为入点的第一个点
	for (it2 = subject_polygonlist.begin(); it2 != subject_polygonlist.end(); it2++)
		if (it2->pointFlag == 1 && it2->inFlag) break;

	subject_polygonClip.pts.clear();
	while (visitall == false) //若plist中所有交点被访问，则跳出循环
	{
		visitall = true;
		for (it3 = plist.begin(); it3 != plist.end(); it3++)
		{
			bool flag = false;//遍历plist中的每一个点观察是否被裁剪
			if (allpoints.pts.size() == 0)
			{
				visitall = false;
			}
			for (int i = 0; i < allpoints.pts.size(); i++)
			{
				//如果找到对应点
				if ((it3->p.x == allpoints.pts[i].x) && (it3->p.y == allpoints.pts[i].y))
				{
					flag = true;
				}

				if (flag == false)
				{
					if (i == allpoints.pts.size() - 1) //遍历到最后还没找到对应点
					{
						visitall = false;
					}
				}
			}
		}

		//全都被访问，直接跳出循环
		if (visitall == true)
		{
			continue;
		}

		//循环遍历subject_polygonlist 
		it2 = (it2 == subject_polygonlist.end()) ? subject_polygonlist.begin() : it2;
		subject_polygonClip.pts.push_back(it2->p);
		allpoints.pts.push_back(it2->p);
		it2++;
		it2 = (it2 == subject_polygonlist.end()) ? subject_polygonlist.begin() : it2;
		
		for (; it2 != subject_polygonlist.end(); it2++)
		{
			//subject_polygonlist遍历到结尾，要从头开始
			if (it2->pointFlag == 1 && !it2->inFlag) break;
			subject_polygonClip.pts.push_back(it2->p);
			allpoints.pts.push_back(it2->p);
		}
		
		//在clip_polygonlist中找同一点
		for (it1 = clip_polygonlist.begin(); it1 != clip_polygonlist.end(); it1++)
			if (it1->p == it2->p) break;
		
		for (; it1 != clip_polygonlist.end(); it1++)
		{
			if (it1->pointFlag == 1 && it1->inFlag) break;
			subject_polygonClip.pts.push_back(it1->p);
			allpoints.pts.push_back(it1->p);
		}
		
		//首尾点相等，找到一个裁剪区域，输出，并绘制
		if (circle(subject_polygonClip, it1))
		{
			//输出subject_polygonClip;
			subject_polygonClip.drawPg();
			subject_polygonClip.pts.clear();
			for (; it2 != subject_polygonlist.end(); it2++) //未到subject_polygonlist结尾，继续在被裁减多边形subject_polygonlist中找到是交点 且为入点的第一个点
				if (it2->pointFlag == 1 && it2->inFlag) break;
			continue;//回到最外层while，判断while (visitall==false)
		}
		
		//在subject_polygonlist中找同一点
		for (; it2 != subject_polygonlist.end(); it2++)
			if (it2->p == it1->p) break;
	}
}

//裁剪函数
void weilerAtherton(Pg& clip_polygon, Pg& subject_polygon)
{
	list<IntersectionPoint> plist, subject_polygonlist, clip_polygonlist;
	generateIntersectPoints(clip_polygon, subject_polygon, plist);//求出所有交点放在plist中
	
	//将交点按序插入多边形的顶点数组中
	generateList(clip_polygon, plist, clip_polygonlist, 0);
	generateList(subject_polygon, plist, subject_polygonlist, 1);
	
	//判断每个交点相对于clip_polygon是出点还是入点
	getsubject_polygonPointInOut(clip_polygon, subject_polygonlist);//裁剪多边形 插入交点的被裁剪多边形链表
	getclip_polygonPointInOut(clip_polygonlist, subject_polygonlist);
	
	//生成裁剪区域
	generateClipAreas(clip_polygonlist, subject_polygonlist, plist);
}

//键盘交互事件
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
}

//绘制内容
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLineWidth(2);
	draw_coordinate();

	//绘制图形
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
	glutCreateWindow("Weiler-Atherton多边形裁剪");

	init();
	
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}