#define GLUT_DISABLE_ATEXIT_HACK
#include <stdlib.h>
#include "glut.h"
#include <math.h>
#include <vector>
#include <list>
#include <algorithm>
#include <map>

using namespace std;

#define epsion 1e-3

struct my_v_inhomogeneous
{
	float x;
	float y;
};

//����α�
struct Edge
{
	double cur_X;//��ǰɨ�����뵱ǰ�߽����x����ֵ
	double reciprocal_K;//��ǰ��б�ʵĵ���
	int max_Y;//��ǰ�����˵�����y����ֵ
	Edge(int y, float x, float rec_k)
	{
		max_Y = y;
		cur_X = x;
		reciprocal_K = rec_k;
	}
};

vector<my_v_inhomogeneous> polygon;
map<int, vector<Edge>> ET;//����yֵ�����б߽��з�����
vector<Edge> AET;//��߱�

bool filling = false;
int y_min, y_max;

/***************************************
* ���ɶ���ε�ET��
* polygon����ʱ�����еĶ�������
* ����polygon,����������֮�乹��һ����Edge����ͬʱ����������y������Сֵ����
���б߽��з���Ͳ�������
***************************************/
void generate_ET()
{
	for (int eindex = 0; eindex < polygon.size(); eindex++)
	{
		my_v_inhomogeneous startPoint = polygon[eindex];
		my_v_inhomogeneous endPoint = (eindex == (polygon.size() - 1)) ? polygon[0] : polygon[eindex + 1];
		if (endPoint.y == startPoint.y) continue;//ˮƽ�߲�����ET��

		//����һ����
		my_v_inhomogeneous top_point = startPoint.y > endPoint.y ? startPoint : endPoint;
		my_v_inhomogeneous bottom_point = startPoint.y < endPoint.y ? startPoint : endPoint;
		float rec_k = (top_point.x - bottom_point.x) / (top_point.y - bottom_point.y);
		Edge one_ET_Edge(top_point.y, bottom_point.x, rec_k);

		//�Ա����˵�y������СֵΪ���ݶԱ߽��з���Ͳ�������
		map<int, vector<Edge>>::iterator it = ET.find(bottom_point.y);
		if (it == ET.end())
		{
			vector<Edge> new_edge_list;
			new_edge_list.push_back(one_ET_Edge);
			ET.insert(pair<int, vector<Edge>>(bottom_point.y, new_edge_list));
		}
		else
		{
			bool inserted = false;
			for (vector<Edge>::iterator eit = it->second.begin(); eit != it->second.end(); eit++)
			{
				if (bottom_point.x < eit->cur_X || (bottom_point.x == eit->cur_X && rec_k < eit->reciprocal_K))
				{
					it->second.insert(eit, one_ET_Edge);
					inserted = true;
					break;
				}
			}
			if (false == inserted)
			{
				it->second.push_back(one_ET_Edge);
			}
		}
	}
}

//ɨ��ת��ֱ�߶�
void DDA(int x0, int y0, int x1, int y1)
{
	/*float dy = y1 - y0;
	float dx = x1 - x0;
	float k = dy / dx;
	float b = y1 - k * x1;
	float xf = x0;
	float yf = y0;
	if (k <= 1 && k >= -1)
	{
		if (xf <= x1)
		{
			while (xf <= x1)
			{
				glVertex2d(xf, yf);
				xf++;
				yf = k * xf + b ;
			}
		}
		else
		{
			while (xf >= x1)
			{
				glVertex2d(x1, y1);
				x1++;
				y1 = k * x1 + b ;
			}
		}

	}
	else if (k > 1 || k < -1)
	{
		if (yf <= y1)
		{
			while (yf <= y1)
			{
				glVertex2d(xf, yf);
				yf++;
				xf = xf + 1 / k ;
			}
		}
		else
		{
			while (y1 <= yf)
			{
				glVertex2d(x1, y1);
				y1++;
				x1 = x1 + (1 / k) ;
			}
		}
	}*/
	int dx = x1 - x0;								//x������
	int dy = y1 - y0;								//y������
	int steps;
	float xIncrement, yIncrement, x = x0, y = y0;
	if (abs(dx) > abs(dy))						    //˭��������
	{
		steps = abs(dx);
	}
	else
	{
		steps = abs(dy);
	}

	xIncrement = float(dx) / float(steps);          //xÿ��������
	yIncrement = float(dy) / float(steps);			//y��ÿ������

	glVertex2f(x0, y0);							//���Ҫ����
	for (int k = 0; k <= steps; ++k)
	{
		x += xIncrement;							//x��+����
		y += yIncrement;							//y��+����
		glVertex2f(x, y);
	}
	glVertex2f(x1, y1);						//�յ㻭��

}








/// //////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
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
	float z = 0;
	float ratio{};
	enum relative_position pos = relative_position::inside_on;
};

//�Զ����������ݽṹ�����ڴ洢��������ཻ�Ľ���
struct IntersectionPoint
{
	my_homogeneous_point_EX p;
	int pointFlag{};     //����Ƿ�Ϊ����: 1Ϊ����  0Ϊԭ�е�
	int index0{}, index1{}; //���ڲü�����ε�index0  ���߶���  �ڱ��ü�����ε�index1���߶���

	bool inFlag{};  //����Ϊfalse  ������Ĭ��Ϊtrue
	int distance{};  //������������߶����ľ��룬�����ڽ��������ʱ��ʹ��
};

//pg��Ķ���(��������㼯�����Ʋü���Ļ�·)
class Pg
{
public:
	vector<my_homogeneous_point_EX> pts;//��������
	Pg(void);
	~Pg(void);
	void drawPg();//���ƻ�·
};
Pg::Pg(void) {}
Pg::~Pg(void) {}
void Pg::drawPg()
{
	/*glBegin(GL_LINE_LOOP);
	for (int i = 0; i < pts.size(); i++)
		glVertex2i(pts[i].x, pts[i].y);
	glEnd();*/

	glBegin(GL_POINTS);
	int i;
	for (i = 0; i < pts.size() - 1; i++)
	{
		DDA((floor)(pts[i].x + 0.5), (floor)(pts[i].y + 0.5),
			(floor)(pts[i + 1].x + 0.5), (floor)(pts[i + 1].y + 0.5));
	}
	DDA(pts[i].x, pts[i].y, pts[0].x, pts[0].y);
	glEnd();
}

Pg clip_polygon, clip_polygon2, subject_polygon, subject_polygon2;
my_homogeneous_point_EX p21, p22, p23, p24;

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

//��ʼ���ü�����κͱ��ü�����εĶ���
void init()
{
	//���ü������
	my_homogeneous_point_EX p01, p02, p03, p04, p05;
	p01.x = 0; p01.y = 200;
	p02.x = -150, p02.y = -150;
	p03.x = 150, p03.y = -150;
	/*p04.x = 60, p04.y = 50;
	p05.x = 0, p05.y = -160;*/
	subject_polygon.pts.push_back(p01);
	subject_polygon.pts.push_back(p02);
	subject_polygon.pts.push_back(p03);
	/*subject_polygon.pts.push_back(p04);
	subject_polygon.pts.push_back(p05);*/

	//�ü������
	my_homogeneous_point_EX p11, p12, p13, p14, p15, p16;
	p11.x = -200; p11.y = 150;
	p12.x = -200, p12.y = -100;
	p13.x = -50, p13.y = -150;
	p14.x = 160, p14.y = -100;
	p15.x = 50, p15.y = -50;
	p16.x = 160, p16.y = 150;

	clip_polygon.pts.push_back(p11);
	clip_polygon.pts.push_back(p12);
	clip_polygon.pts.push_back(p13);
	clip_polygon.pts.push_back(p14);
	clip_polygon.pts.push_back(p15);
	clip_polygon.pts.push_back(p16);


	p21.x = -120, p21.y = 50;
	p22.x = -40, p22.y = 50;
	p23.x = -40, p23.y = -30;
	p24.x = -120, p24.y = -30;

	clip_polygon2.pts.push_back(p24);
	clip_polygon2.pts.push_back(p23);
	clip_polygon2.pts.push_back(p22);
	clip_polygon2.pts.push_back(p21);
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

//���  ֱ��p0p1��p0p2
int cross(my_homogeneous_point_EX& p0, my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2)
{
	return ((p2.x - p0.x) * (p1.y - p0.y) - (p1.x - p0.x) * (p2.y - p0.y));
}

//�жϵ�p2�Ƿ����߶�p0,p1֮��  p0,p1,p2��ͬһֱ����
bool onSegment(my_homogeneous_point_EX& p0, my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2)
{
	int minx = min(p0.x, p1.x), maxx = max(p0.x, p1.x);
	int miny = min(p0.y, p1.y), maxy = max(p0.y, p1.y);
	if (p2.x >= minx && p2.x <= maxx && p2.y >= miny && p2.y <= maxy) return true;
	return false;
}

//�ж�����ֱ���Ƿ��н���
bool segmentsIntersect(my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2, my_homogeneous_point_EX& p3, my_homogeneous_point_EX& p4)
{
	int d1 = cross(p3, p4, p1);
	int d2 = cross(p3, p4, p2);
	int d3 = cross(p1, p2, p3);
	int d4 = cross(p1, p2, p4);
	if (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
		((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0)))
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
	int clip_polygonSize = clip_polygon.pts.size(), subject_polygonSize =
		subject_polygon.pts.size();
	for (int i = 0; i < clip_polygonSize; i++) {
		my_homogeneous_point_EX p1 = clip_polygon.pts[i];
		my_homogeneous_point_EX p2 = clip_polygon.pts[(i + 1) % clip_polygonSize];
		for (int j = 0; j < subject_polygonSize; j++) {
			my_homogeneous_point_EX p3 = subject_polygon.pts[j];
			my_homogeneous_point_EX p4 = subject_polygon.pts[(j + 1) %
				subject_polygonSize];
			//�ж����߶��Ƿ��н���
			if (segmentsIntersect(p1, p2, p3, p4)) {
				IntersectionPoint ip;
				ip.index0 = i;//���ڲü�����ε�i��ֱ����
				ip.index1 = j;//���ڱ��ü�����ε�j��ֱ����
				ip.p = getintersectPoint(p1, p2, p3, p4);//���߶εĽ���
				plist.push_back(ip);
			}
		}
	}
}

//��p1 p2��ľ���  ƽ��
int getDistance(my_homogeneous_point_EX& p1, my_homogeneous_point_EX& p2)
{
	return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

//����Ƚ�
bool distanceComparator(IntersectionPoint& ip1, IntersectionPoint& ip2)
{
	return ip1.distance < ip2.distance;
}

//��plist�еĵ㰴�����comlist��  indexΪ0��ʾ�ü�����Σ�indexΪ1��ʾ���ü������
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
			if ((index == 0 && i == it->index0) ||
				(index == 1 && i == it->index1))
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

//�жϵ�����㻹�ǳ���  ����Ϊfalse  ������Ĭ��Ϊtrue
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

//�������
void getsubject_polygonPointInOut2(Pg& clip_polygon, list<IntersectionPoint>& subject_polygonlist)
{
	bool inFlag = false;
	list<IntersectionPoint>::iterator it;
	for (it = subject_polygonlist.begin(); it != subject_polygonlist.end(); it++)
	{
		if (it->pointFlag == 0) //�ǽ�������
		{
			inFlag = !isPointInside(it->p, clip_polygon);
		}
		else //��������
		{
			inFlag = !inFlag;
			it->inFlag = inFlag;
		}
	}
}

//�ж�p1 p2  �Ƿ�Ϊͬһ��  ������� 
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
			if (it2->pointFlag == 0) continue;
			//  ��Ϊ�����ڲü�����κͱ��ü�����ζ�����  ����ͨ��ƥ��ĸ�clip_polygonlist��Ľ��㸳��㡢�����ֵ
			if (it1->p == it2->p) it1->inFlag = it2->inFlag;
		}
	}
}

//�������ü�����
void generateClipAreas(list<IntersectionPoint>& clip_polygonlist, list<IntersectionPoint>& subject_polygonlist, list<IntersectionPoint>& plist)
{
	list<IntersectionPoint>::iterator it1, it2;
	list<IntersectionPoint>::iterator it3;  //it3�����ȽϽ����Ƿ񶼱�����
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
			//����
			vector<my_homogeneous_point_EX>::iterator it;
			for (it = subject_polygonClip.pts.begin(); it != subject_polygonClip.pts.end(); it++)
				subject_polygon2.pts.push_back(*it);
			//���subject_polygonClip;
			//subject_polygonClip.drawPg();
			subject_polygonClip.pts.clear();
			for (; it2 != subject_polygonlist.end(); it2++) //δ��subject_polygonlist��β�������ڱ��ü������subject_polygonlist���ҵ��ǽ���  ��Ϊ���ĵ�һ����
				if (it2->pointFlag == 1 && it2->inFlag) break;
			continue;//�ص������while���ж�while (visitall==false)
		}
		//��subject_polygonlist����ͬһ��
		for (; it2 != subject_polygonlist.end(); it2++)
			if (it2->p == it1->p) break;
	}
}

//�����ڿ�ü�����
void generateClipAreas2(list<IntersectionPoint>& clip_polygonlist, list<IntersectionPoint>& subject_polygonlist, list<IntersectionPoint>& plist)
{
	list<IntersectionPoint>::iterator it1, it2;
	it2 = subject_polygonlist.begin();
	list<IntersectionPoint>::iterator it3;  //it3�����ȽϽ����Ƿ񶼱�����
	Pg subject_polygonClip; //��ʱ�ü����
	subject_polygonClip.pts.clear();
	Pg allpoints; //������¼���б������ĵ� 
	bool visitall = false;
	//�ڲü������clip_polygonlist���ҵ��ǽ�����Ϊ���ĵ�һ����
	for (it1 = clip_polygonlist.begin(); it1 != clip_polygonlist.end(); it1++)
		if (it1->pointFlag == 1 && it1->inFlag)
			break;

	while (!visitall)
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

		//clip����㵽����
		for (; !(it1->pointFlag && !it1->inFlag);)
		{
			subject_polygonClip.pts.push_back(it1->p);
			allpoints.pts.push_back(it1->p);
			it1++;
			if (it1 == clip_polygonlist.end())
				it1 = clip_polygonlist.begin();
		}
		//subject����ͬ����
		for (; !(it1->p == it2->p);)
		{
			it2++;
			if (it2 == subject_polygonlist.end())
				it2 = subject_polygonlist.begin();
		}
		//subject�ӳ��㵽���
		for (; !(it2->pointFlag && it2->inFlag);)
		{
			subject_polygonClip.pts.push_back(it2->p);
			allpoints.pts.push_back(it2->p);
			it2++;
			if (it2 == subject_polygonlist.end())
				it2 = subject_polygonlist.begin();
		}
		//clip�ҵ���ͬ���
		for (; !(it1->p == it2->p);)
		{
			it1++;
			if (it1 == clip_polygonlist.end())
				it1 = clip_polygonlist.begin();
		}
	}
	/////////////////////////////////////////////////////////////
	my_v_inhomogeneous p{};
	vector<my_homogeneous_point_EX>::iterator it;
	it = subject_polygonClip.pts.begin();
	p.x = it->x;
	p.y = it->y;
	y_min = p.y;
	y_max = p.y;
	//polygon.clear();
	polygon.push_back(p);
	it++;
	for (; it != subject_polygonClip.pts.end(); it++)
	{
		if (!(p.x == it->x && p.y == it->y))
		{
			p.x = it->x;
			p.y = it->y;
			y_min = p.y < y_min ? p.y : y_min;
			y_max = p.y > y_max ? p.y : y_max;
			polygon.push_back(p);
		}
	}
	generate_ET();
	/////////////////////////////////////////////////////////////
	subject_polygonClip.drawPg();
}

//���ü�����
void weilerAtherton(Pg& clip_polygon, Pg& subject_polygon)
{
	list<IntersectionPoint> plist, subject_polygonlist, clip_polygonlist;
	generateIntersectPoints(clip_polygon, subject_polygon, plist);//������н������plist��
	//�����㰴��������εĶ���������
	generateList(clip_polygon, plist, clip_polygonlist, 0);
	generateList(subject_polygon, plist, subject_polygonlist, 1);
	//�ж�ÿ�����������clip_polygon�ǳ��㻹�����
	getsubject_polygonPointInOut(clip_polygon, subject_polygonlist);//�ü������  ���뽻��ı��ü����������
	getclip_polygonPointInOut(clip_polygonlist, subject_polygonlist);
	//���ɲü�����
	generateClipAreas(clip_polygonlist, subject_polygonlist, plist);
}

//�ڿ���к���
void weilerAtherton2(Pg& clip_polygon, Pg& subject_polygon)
{
	list<IntersectionPoint> plist, subject_polygonlist, clip_polygonlist;
	generateIntersectPoints(clip_polygon, subject_polygon, plist);//������н������plist��
	//�����㰴��������εĶ���������
	generateList(clip_polygon, plist, clip_polygonlist, 0);
	generateList(subject_polygon, plist, subject_polygonlist, 1);
	//�ж�ÿ�����������clip_polygon�ǳ��㻹�����
	getsubject_polygonPointInOut(clip_polygon, subject_polygonlist);//�ü������  ���뽻��ı��ü����������
	getclip_polygonPointInOut(clip_polygonlist, subject_polygonlist);

	//���ɲü�����
	list<IntersectionPoint> clip_polygonlist2;
	list<IntersectionPoint>::iterator it;
	it = clip_polygonlist.end();
	it--;
	for (; it != clip_polygonlist.begin(); it--)
		clip_polygonlist2.push_back(*it);
	clip_polygonlist2.push_back(*it);
	generateClipAreas2(clip_polygonlist2, subject_polygonlist, plist);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////






//��ʼ��

//void generate_polygon_points()
//{
//	my_v_inhomogeneous p01;
//	p01.x = -200, p01.y = -180;
//	y_min = p01.y;
//	y_max = p01.y;
//	polygon.push_back(p01);
//
//	my_v_inhomogeneous p02;
//	p02.x = 50, p02.y = -250;
//	y_min = p02.y < y_min ? p02.y : y_min;
//	y_max = p02.y > y_max ? p02.y : y_max;
//	polygon.push_back(p02);
//	
//	my_v_inhomogeneous p03;
//	p03.x = 200, p03.y = -50;
//	y_min = p03.y < y_min ? p03.y : y_min;
//	y_max = p03.y > y_max ? p03.y : y_max;
//	polygon.push_back(p03);
//	
//	my_v_inhomogeneous p04;
//	p04.x = 200, p04.y = 220;
//	y_min = p04.y < y_min ? p04.y : y_min;
//	y_max = p04.y > y_max ? p04.y : y_max;
//	polygon.push_back(p04);
//	
//	my_v_inhomogeneous p05;
//	p05.x = -50, p05.y = 100;
//	y_min = p05.y < y_min ? p05.y : y_min;
//	y_max = p05.y > y_max ? p05.y : y_max;
//	polygon.push_back(p05);
//	
//	my_v_inhomogeneous p06;
//	p06.x = -200, p06.y = 200;
//	y_min = p06.y < y_min ? p06.y : y_min;
//	y_max = p06.y > y_max ? p06.y : y_max;
//	polygon.push_back(p06);
//}

/***************************************
* ��AET�в����µ�Edge���ϣ�������
* polygon����ʱ�����еĶ�������
***************************************/
void insert_new_edges_into_AET(vector<Edge> newedges)
{
	//����AET�бߵ�������򣬶�ÿ���߽��в�������
	for (vector<Edge>::iterator newHead = newedges.begin(); newHead != newedges.end(); newHead++)
	{
		bool inserted = false;
		for (vector<Edge>::iterator AET_Head = AET.begin(); AET_Head != AET.end(); AET_Head++)
		{
			if (newHead->cur_X < AET_Head->cur_X || (newHead->cur_X == AET_Head->cur_X && newHead->reciprocal_K < AET_Head->reciprocal_K))
			{
				AET.insert(AET_Head, *newHead);
				inserted = true;
				break;
			}
		}
		if (false == inserted)
		{
			AET.push_back(*newHead);
		}
	}
}

/***************************************
* ɨ�������
***************************************/
void scanline_filling()
{
	AET.clear();
	glColor3f(0.0f, 1.0f, 0.0f);
	for (int ypos = y_min; ypos <= y_max; ypos += 1)//���б�������ɨ����
	{
		//�ҵ��������˵�y������СֵΪ�뵱ǰɨ����ypos��ͬ�����б�
		map<int, vector<Edge>>::iterator it = ET.find(ypos);
		if (it != ET.end())
		{
			insert_new_edges_into_AET(it->second);//���뵽AET����
		}

		vector<int> need_removed_edge_index;//�����Ҫɾ���ı����

		//ȷ������߶��������
		//glBegin(GL_LINES);
		glBegin(GL_POINTS);
		for (int eindex = 0; eindex < AET.size(); eindex += 2)
		{
			float x;
			if (AET[eindex].reciprocal_K > 1)
			{
				//glVertex2f(AET[eindex].cur_X + 2, ypos);
				x = AET[eindex].cur_X + 2;
			}
			else
			{
				//glVertex2f(AET[eindex].cur_X + 1, ypos);
				x = AET[eindex].cur_X + 1;
			}
			AET[eindex].cur_X += 1 * AET[eindex].reciprocal_K;

			//glVertex2f(AET[eindex + 1].cur_X - 1, ypos);
			DDA(x, ypos, AET[eindex + 1].cur_X - 1, ypos);
			//+0.5
			//DDA((floor)(x + 0.5), ypos, (floor)(AET[eindex + 1].cur_X - 1 + 0.5), ypos);
			//����
			//DDA((floor)(x), ypos, (floor)(AET[eindex + 1].cur_X - 1 + 2), ypos);
			AET[eindex + 1].cur_X += 1 * AET[eindex + 1].reciprocal_K;

			if (AET[eindex].max_Y == ypos + 1)
				need_removed_edge_index.push_back(eindex);

			if (AET[eindex + 1].max_Y == ypos + 1)
				need_removed_edge_index.push_back(eindex + 1);
		}
		glEnd();

		for (int eindex = need_removed_edge_index.size() - 1; eindex >= 0; eindex--)
		{
			AET.erase(AET.begin() + need_removed_edge_index[eindex]);
		}
	}
}

void display()
{
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(2);
	draw_coordinate();

	//����ͼ�� 
	glColor3f(0.0f, 0.0f, 0.0f);

	clip_polygon.drawPg();
	clip_polygon2.drawPg();

	glColor3f(0.0f, 0.0f, 1.0f);

	if (true == clip)
	{
		weilerAtherton(clip_polygon, subject_polygon);
		weilerAtherton2(clip_polygon2, subject_polygon2);
	}
	else
	{
		subject_polygon.drawPg();
	}

	if (filling == true)
	{
		scanline_filling();
		polygon.clear();
		/*weilerAtherton(clip_polygon, subject_polygon);
		weilerAtherton2(clip_polygon2, subject_polygon2);*/
	}

	glutSwapBuffers();
}

//ͶӰ��ʽ��modelview��ʽ����
void reshape(int w, int h)
{
	//glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//if (w <= h)
	//	glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height *
	//		(GLfloat)nearplane_height / (GLfloat)nearplane_width, 0.5 * nearplane_height *
	//		(GLfloat)nearplane_height / (GLfloat)nearplane_width,
	//		-nearplane_distance, farplane_distance); //������ӵ�
	//else
	//	glOrtho(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height *
	//		(GLfloat)nearplane_width / (GLfloat)nearplane_height, 0.5 * nearplane_height *
	//		(GLfloat)nearplane_width / (GLfloat)nearplane_height,
	//		-nearplane_distance, farplane_distance);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glViewport(0, 0, w, h); //��������Ӿ����ȡ�ĳ������������ĸߺͿ���ʾ����Ļ��
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-0.5 * w, 0.5 * w, -0.5 * h, 0.5 * h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
		filling = true;
		glutPostRedisplay();
		break;
	}
	case'3':
	{
		filling = false;
		glutPostRedisplay();
		break;
	}
	case'4':
	{
		clip = false;
		glutPostRedisplay();
		break;
	}
	case 'w':
	case 'W':
	{
		clip_polygon2.pts.clear();
		p21.y += 10;
		p22.y += 10;
		p23.y += 10;
		p24.y += 10;

		clip_polygon2.pts.push_back(p24);
		clip_polygon2.pts.push_back(p23);
		clip_polygon2.pts.push_back(p22);
		clip_polygon2.pts.push_back(p21);
		glutPostRedisplay();
		break;
	}

	case 's':
	case 'S':
	{
		clip_polygon2.pts.clear();
		p21.y -= 10;
		p22.y -= 10;
		p23.y -= 10;
		p24.y -= 10;

		clip_polygon2.pts.push_back(p24);
		clip_polygon2.pts.push_back(p23);
		clip_polygon2.pts.push_back(p22);
		clip_polygon2.pts.push_back(p21);
		glutPostRedisplay();
		break;
	}
	case'a':
	{
		clip_polygon2.pts.clear();
		p21.x -= 10;
		p22.x -= 10;
		p23.x -= 10;
		p24.x -= 10;

		clip_polygon2.pts.push_back(p24);
		clip_polygon2.pts.push_back(p23);
		clip_polygon2.pts.push_back(p22);
		clip_polygon2.pts.push_back(p21);
		glutPostRedisplay();
		break;
	}
	case'd':
	{
		clip_polygon2.pts.clear();
		p21.x += 10;
		p22.x += 10;
		p23.x += 10;
		p24.x += 10;

		clip_polygon2.pts.push_back(p24);
		clip_polygon2.pts.push_back(p23);
		clip_polygon2.pts.push_back(p22);
		clip_polygon2.pts.push_back(p21);
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

int main(int argc, char** argv)
{
	//generate_polygon_points();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(nearplane_width, nearplane_height);
	glutInitWindowPosition(100, 100);
	//glutCreateWindow(argv[0]);
	glutCreateWindow("1��2��3����4����");

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}