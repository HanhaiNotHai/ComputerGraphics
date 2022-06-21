//ObjLoader.h
#pragma once
#include <vector>
#include <string> 
#include <iostream>
#include <fstream>

using namespace std;

//纹理坐标数据结构
struct my_2D_Texture_coord
{
	float u{};//u方向上的坐标
	float v{};//v方向上的坐标

	my_2D_Texture_coord() {}
	~my_2D_Texture_coord() {}

	my_2D_Texture_coord(float ui, float vi)
	{
		u = ui;
		v = vi;
	}
};

//顶点数据结构
struct my_3D_point_coord
{
	float x{};//x轴坐标值
	float y{};//y轴坐标值
	float z{};//z轴坐标值

	my_3D_point_coord() {}
	~my_3D_point_coord() {}

	my_3D_point_coord(float xi, float yi, float zi)
	{
		x = xi;
		y = yi;
		z = zi;
	}
};

//面数据结构，即构成三维图形的一个三角网格面
struct my_triangle_indices
{
	int first_point_index;//第一个点序号
	int first_point_texture_index;//第一个纹理坐标序号
	int first_point_normal_index;//第一个点法向序号

	int second_point_index;//第二个点序号
	int second_point_texture_index;//第二个纹理坐标序号
	int second_point_normal_index;//第二个点法向序号

	int third_point_index;//第三个点序号
	int third_point_texture_index;//第三个纹理坐标序号
	int third_point_normal_index;//第三个点法向序号
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

//三维空间中的三角网格模型
struct my_triangle_3DModel
{
	float transparency = 0;
	float reflection = 0;

	//以下用于计算直接光照能量
	float material_ambient_rgb_reflection[3] = { 0,0,0 }; //环境光反射系数，初始不反射
	float material_diffuse_rgb_reflection[3] = { 0,0,0 };//漫反射系数，初始不反射
	float material_specular_rgb_reflection[3] = { 0,0,0 };//镜面光反射系数，初始不反射
	float ns = 0;//聚光指数，初始不聚光

	vector<my_3D_point_coord> pointSets;//存放模型所有顶点
	vector<my_3Dvector> pointNormalSets;//存放模型所有顶点的法向
	vector<my_2D_Texture_coord> pointTextureSets;//存放模型所有纹理坐标
	vector<my_triangle_indices> faceSets;//存放模型所有三角网格面

	//模型尺寸
	float max_x = -1e8, min_x = 1e8;
	float max_y = -1e8, min_y = 1e8;
	float max_z = -1e8, min_z = 1e8;

	void modify_color_configuration(float transparency, float reflection, float ambient_reflection[], float diffuse_reflection[], float specular_reflection[], float ns)
	{
		this->transparency = transparency;
		this->reflection = reflection;

		//以下用于计算直接光照能量 memcpy用于把资源内存拷到目标内存中
		memcpy(this->material_ambient_rgb_reflection, ambient_reflection, 3 * sizeof(float));
		memcpy(this->material_diffuse_rgb_reflection, diffuse_reflection, 3 * sizeof(float));
		memcpy(this->material_specular_rgb_reflection, specular_reflection, 3 * sizeof(float));
		this->ns = ns;
	}
};

//模型加载器
class ObjLoader
{
public:
	my_triangle_3DModel my_3DModel;
public:
	ObjLoader() {}
	ObjLoader(string filename) //构造函数
	{
		string line;
		fstream f;
		f.open(filename, ios::in);
		if (!f.is_open()) {
			cout << "Something Went Wrong When Opening Objfiles" << endl;
		}

		while (!f.eof())
		{
			getline(f, line);//拿到obj文件中一行，作为一个字符串
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
					parameters.push_back(ans); //取出字符串中的元素，以空格切分
					ans = "";
				}
			}

			if (parameters.size() == 4 || parameters.size() == 3)
			{
				if (parameters[0] == "v") //顶点
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
				else if (parameters[0] == "vn") //顶点的法向量
				{
					my_3Dvector curPointNormal;
					curPointNormal.dx = atof(parameters[1].c_str());
					curPointNormal.dy = atof(parameters[2].c_str());
					curPointNormal.dz = atof(parameters[3].c_str());
					my_3DModel.pointNormalSets.push_back(curPointNormal);
				}
				else if (parameters[0] == "vt") //纹理坐标
				{
					my_2D_Texture_coord curTextureCoord;
					curTextureCoord.u = atof(parameters[1].c_str());
					curTextureCoord.v = atof(parameters[2].c_str());
					my_3DModel.pointTextureSets.push_back(curTextureCoord);
				}
				else if (parameters[0] == "f") //面，顶点索引/纹理 uv 索引/法向索引
				{
					//因为顶点索引在obj文件中是从1开始的，而我们存放的顶点vector是从0开始的，因此要减1
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