//ObjLoader.h
#pragma once
#include <vector>
#include <string> 
#include <iostream>
#include <fstream>

using namespace std;

//�����������ݽṹ
struct my_2D_Texture_coord
{
	float u{};//u�����ϵ�����
	float v{};//v�����ϵ�����

	my_2D_Texture_coord() {}
	~my_2D_Texture_coord() {}

	my_2D_Texture_coord(float ui, float vi)
	{
		u = ui;
		v = vi;
	}
};

//�������ݽṹ
struct my_3D_point_coord
{
	float x{};//x������ֵ
	float y{};//y������ֵ
	float z{};//z������ֵ

	my_3D_point_coord() {}
	~my_3D_point_coord() {}

	my_3D_point_coord(float xi, float yi, float zi)
	{
		x = xi;
		y = yi;
		z = zi;
	}
};

//�����ݽṹ����������άͼ�ε�һ������������
struct my_triangle_indices
{
	int first_point_index;//��һ�������
	int first_point_texture_index;//��һ�������������
	int first_point_normal_index;//��һ���㷨�����

	int second_point_index;//�ڶ��������
	int second_point_texture_index;//�ڶ��������������
	int second_point_normal_index;//�ڶ����㷨�����

	int third_point_index;//�����������
	int third_point_texture_index;//�����������������
	int third_point_normal_index;//�������㷨�����
};

class my_3Dvector
{
public:
	float dx{};
	float dy{};
	float dz{};
	float len{};
public:
	my_3Dvector() {}
	~my_3Dvector() {}

	my_3Dvector(float x, float y, float z)
	{
		dx = x;
		dy = y;
		dz = z;
		len = sqrtf(powf(dx, 2) + powf(dy, 2) + powf(dz, 2));
	}
};

//��ά�ռ��е���������ģ��
struct my_triangle_3DModel
{
	float transparency = 0;
	float reflection = 0;

	//�������ڼ���ֱ�ӹ�������
	float material_ambient_rgb_reflection[3] = { 0,0,0 }; //�����ⷴ��ϵ������ʼ������
	float material_diffuse_rgb_reflection[3] = { 0,0,0 };//������ϵ������ʼ������
	float material_specular_rgb_reflection[3] = { 0,0,0 };//����ⷴ��ϵ������ʼ������
	float ns = 0;//�۹�ָ������ʼ���۹�

	vector<my_3D_point_coord> pointSets;//���ģ�����ж���
	vector<my_3Dvector> pointNormalSets;//���ģ�����ж���ķ���
	vector<my_2D_Texture_coord> pointTextureSets;//���ģ��������������
	vector<my_triangle_indices> faceSets;//���ģ����������������

	//ģ�ͳߴ�
	float max_x = -1e8, min_x = 1e8;
	float max_y = -1e8, min_y = 1e8;
	float max_z = -1e8, min_z = 1e8;

	void modify_color_configuration(float transparency, float reflection, float ambient_reflection[], float diffuse_reflection[], float specular_reflection[], float ns)
	{
		this->transparency = transparency;
		this->reflection = reflection;

		//�������ڼ���ֱ�ӹ������� memcpy���ڰ���Դ�ڴ濽��Ŀ���ڴ���
		memcpy(this->material_ambient_rgb_reflection, ambient_reflection, 3 * sizeof(float));
		memcpy(this->material_diffuse_rgb_reflection, diffuse_reflection, 3 * sizeof(float));
		memcpy(this->material_specular_rgb_reflection, specular_reflection, 3 * sizeof(float));
		this->ns = ns;
	}
};

//ģ�ͼ�����
class ObjLoader
{
public:
	my_triangle_3DModel my_3DModel;
public:
	ObjLoader() {}
	ObjLoader(string filename) //���캯��
	{
		string line;
		fstream f;
		f.open(filename, ios::in);
		if (!f.is_open()) {
			cout << "Something Went Wrong When Opening Objfiles" << endl;
		}

		while (!f.eof())
		{
			getline(f, line);//�õ�obj�ļ���һ�У���Ϊһ���ַ���
			if (line.find("#") != -1)
				continue;

			line.append(" ");
			vector<string> parameters;
			string ans = "";

			for (unsigned int i = 0; i < line.length(); i++)
			{
				char ch = line[i];
				if (ch != ' ')
				{
					ans += ch;
				}
				else if (ans != "")
				{
					parameters.push_back(ans); //ȡ���ַ����е�Ԫ�أ��Կո��з�
					ans = "";
				}
			}

			if (parameters.size() == 4 || parameters.size() == 3)
			{
				if (parameters[0] == "v") //����
				{
					my_3D_point_coord curPoint;

					curPoint.x = atof(parameters[1].c_str());
					my_3DModel.max_x = my_3DModel.max_x < curPoint.x ? curPoint.x : my_3DModel.max_x;
					my_3DModel.min_x = my_3DModel.min_x > curPoint.x ? curPoint.x : my_3DModel.min_x;

					curPoint.y = atof(parameters[2].c_str());
					my_3DModel.max_y = my_3DModel.max_y < curPoint.y ? curPoint.y : my_3DModel.max_y;
					my_3DModel.min_y = my_3DModel.min_y > curPoint.y ? curPoint.y : my_3DModel.min_y;

					curPoint.z = atof(parameters[3].c_str());
					my_3DModel.max_z = my_3DModel.max_z < curPoint.z ? curPoint.z : my_3DModel.max_z;
					my_3DModel.min_z = my_3DModel.min_z > curPoint.z ? curPoint.z : my_3DModel.min_z;

					my_3DModel.pointSets.push_back(curPoint);
				}
				else if (parameters[0] == "vn") //����ķ�����
				{
					my_3Dvector curPointNormal;
					curPointNormal.dx = atof(parameters[1].c_str());
					curPointNormal.dy = atof(parameters[2].c_str());
					curPointNormal.dz = atof(parameters[3].c_str());
					my_3DModel.pointNormalSets.push_back(curPointNormal);
				}
				else if (parameters[0] == "vt") //��������
				{
					my_2D_Texture_coord curTextureCoord;
					curTextureCoord.u = atof(parameters[1].c_str());
					curTextureCoord.v = atof(parameters[2].c_str());
					my_3DModel.pointTextureSets.push_back(curTextureCoord);
				}
				else if (parameters[0] == "f") //�棬��������/���� uv ����/��������
				{
					//��Ϊ����������obj�ļ����Ǵ�1��ʼ�ģ������Ǵ�ŵĶ���vector�Ǵ�0��ʼ�ģ����Ҫ��1
					my_triangle_indices curTri;

					curTri.first_point_index = atoi(parameters[1].substr(0, parameters[1].find_first_of('/')).c_str()) - 1;
					parameters[1] = parameters[1].substr(parameters[1].find_first_of('/') + 1);
					curTri.first_point_texture_index = atoi(parameters[1].substr(0, parameters[1].find_first_of('/')).c_str()) - 1;
					parameters[1] = parameters[1].substr(parameters[1].find_first_of('/') + 1);
					curTri.first_point_normal_index = atoi(parameters[1].substr(0, parameters[1].find_first_of('/')).c_str()) - 1;

					curTri.second_point_index = atoi(parameters[2].substr(0, parameters[2].find_first_of('/')).c_str()) - 1;
					parameters[2] = parameters[2].substr(parameters[2].find_first_of('/') + 1);
					curTri.second_point_texture_index = atoi(parameters[2].substr(0, parameters[2].find_first_of('/')).c_str()) - 1;
					parameters[2] = parameters[2].substr(parameters[2].find_first_of('/') + 1);
					curTri.second_point_normal_index = atoi(parameters[2].substr(0, parameters[2].find_first_of('/')).c_str()) - 1;

					curTri.third_point_index = atoi(parameters[3].substr(0, parameters[3].find_first_of('/')).c_str()) - 1;
					parameters[3] = parameters[3].substr(parameters[3].find_first_of('/') + 1);
					curTri.third_point_texture_index = atoi(parameters[3].substr(0, parameters[3].find_first_of('/')).c_str()) - 1;
					parameters[3] = parameters[3].substr(parameters[3].find_first_of('/') + 1);
					curTri.third_point_normal_index = atoi(parameters[3].substr(0, parameters[3].find_first_of('/')).c_str()) - 1;

					my_3DModel.faceSets.push_back(curTri);
				}
			}
		}
		f.close();
	}
};