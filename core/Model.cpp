#include <io.h> 
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "./Model.h"

Model::Model(const char* filename)
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	if (in.fail())
	{
		printf("load model failed\n");
		return;
	}
	std::string line;
	while (!in.eof())
	{
		std::getline(in, line);
		std::istringstream iss(line.c_str());
		char trash;
		if (line.compare(0, 2, "v ") == 0)//从索引0开始，包含2个字符，等于0表示相同
		{
			iss >> trash;
			Vec3 v;
			for (int i = 0; i < 3; i++)
				iss >> v[i];
			vertVector.push_back(v);//v是三个元素，verts的每一个元素是3个，即每个元素表示.obj中的一行
		}
		else if (line.compare(0, 3, "vt ") == 0)
		{
			iss >> trash >> trash;//分别是v和t字符
			Vec2 vt;
			for (int i = 0; i < 2; i++)//.obj文件中vt后面有多个空格，没关系
				iss >> vt[i];
			uvVector.push_back(vt);//uvs表示每一行vt的集合
		}
		else if (line.compare(0, 3, "vn ") == 0)
		{
			iss >> trash >> trash;
			Vec3 vn;
			for (int i = 0; i < 3; i++)
				iss >> vn[i];//raw[i]代表vn.x/vn.y和vn.z
			normalVector.push_back(vn);//存所有normal的集合，之后用索引在normals容器中查找
		}
		else if (line.compare(0, 2, "f ") == 0)
		{
			//f 24/1/24 25/2/25 26/3/26
			std::vector<int> f;
			int indexVertex, indexUV, indexNormal;
			std::vector<int> faceUV;//容器由三个索引组成，对应这个三角形三个点的uv值
			iss >> trash;//trash = f
			//indexVertex=24 trash=/ indexUV=1 trash=/ indexNormal=24，f中每一行执行三次循环
			while (iss >> indexVertex >> trash >> indexUV >> trash >> indexNormal)
			{
				indexVertex--; // in wavefront obj all indices start at 1, not zero
				f.push_back(indexVertex);//1对应的是vector<Vec3f> verts_数组中的0索引
				indexUV--;//索引也是从1开始累计的，放到容器中从0开始
				f.push_back(indexUV);//每个f中加入三个顶点的uv索引，现在f有六个元素
				indexNormal--;
				f.push_back(indexNormal);
			}
			faceVector.push_back(f);//f是9个元素，组成一个面，faces是所有的面集合,faces中每个元素即.obj中每一行
		}
	}
	std::cerr << "# v# " << vertVector.size() << " vt# " << uvVector.size() << " f# " << faceVector.size() << std::endl;

	//测试：为elfgirl提供特例
	create_map(filename);
	//if (strcmp(filename, "obj/elfgirl/base.obj") == 0)
	//{
	//	diffuseMap = new TGAImage();
	//	load_texture(filename, "_diffuse.tga", diffuseMap);
	//}
	//else if (strcmp(filename, "obj/elfgirl/body0.obj")==0 ||
	//	strcmp(filename, "obj/elfgirl/body1.obj")==0 ||
	//	strcmp(filename, "obj/elfgirl/body2.obj") == 0)
	//{
	//	diffuseMap = new TGAImage();
	//	load_texture("obj/elfgirl/body.obj", "_diffuse.tga", diffuseMap);
	//}
	//else if (strcmp(filename, "obj/elfgirl/face0.obj") == 0 ||
	//	strcmp(filename, "obj/elfgirl/face1.obj") == 0)
	//{
	//	diffuseMap = new TGAImage();
	//	load_texture("obj/elfgirl/face.obj", "_diffuse.tga", diffuseMap);
	//}
	//else if (strcmp(filename, "obj/elfgirl/hair.obj") == 0)
	//{
	//	diffuseMap = new TGAImage();
	//	load_texture("obj/elfgirl/hair.obj", "_diffuse.tga", diffuseMap);
	//}
}

void Model::create_map(const char* filename)
{
	diffuseMap = NULL;
	normalMap = NULL;

	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");

	texfile = texfile.substr(0, dot) + std::string("_diffuse.tga");
	if (_access(texfile.data(), 0) != -1)//windows API 看下有无这个文件
	{
		diffuseMap = new TGAImage();
		load_texture(filename, "_diffuse.tga", diffuseMap);
	}

	texfile = texfile.substr(0, dot) + std::string("_normal.tga");
	if (_access(texfile.data(), 0) != -1)
	{
		normalMap = new TGAImage();
		load_texture(filename, "_normal.tga", normalMap);
	}
}

void Model::load_texture(std::string filename, const char* suffix, TGAImage* img)
{
	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");
	if (dot != std::string::npos)
	{
		texfile = texfile.substr(0, dot) + std::string(suffix);//filename去掉后缀.obj + suffix名称
		std::cerr << "texture file " << texfile << " loading " << (img->read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
		img->flip_vertically();
	}
}

Model::~Model()
{

}

int Model::nfaces()
{
	return (int)faceVector.size();
}

Vec3 Model::GetVertPos(int iface, int nthvert)
{
	std::vector<int> face = faceVector[iface];//f中一行
	int index = face[nthvert * 3];//0 3 6
	Vec3 vertPos = vertVector[index];
	return vertPos;
}
Vec2 Model::GetVertUV(int iface, int nthvert)
{
	std::vector<int> face = faceVector[iface];
	int index = 1 + nthvert * 3;
	int uvIndex = face[index];
	Vec2 uv = uvVector[uvIndex];
	return uv;
}
Vec3 Model::GetVertNormal(int iface, int nthvert)
{
	std::vector<int> face = faceVector[iface];
	int index = 2 + nthvert * 3;
	int normalIndex = face[index];
	Vec3 normal = normalVector[normalIndex];
	return normal;
}
//TGAColor Model::SamplerDiffseColor(Vec2 uv)
//{
//	int diffuseMapW = diffuseMap.get_width();
//	int diffuseMapH = diffuseMap.get_height();
//	return diffuseMap.get((int)(diffuseMapW * uv.x), (int)(diffuseMapH * uv.y));
//}
Vec3 Model::SamplerDiffseColor(Vec2 uv)
{
	uv.x = fmod(uv.x, 1); uv.y = fmod(uv.y, 1);//求余运算
	int diffuseMapW = diffuseMap->get_width();
	int diffuseMapH = diffuseMap->get_height();
	TGAColor color = diffuseMap->get((int)(diffuseMapW * uv.x), (int)(diffuseMapH * uv.y));//[0,255]

	Vec3 result;
	for (int i = 0; i < 3; i++)
		result[2 - i] = (float)color[i] / 255.0;//b, g, r, a顺序
	return result;//[0.0,1.0]
}




std::vector<int> Model::face(int idx)
{
	return faceVector[idx];
}
Vec3 Model::vert(int i)
{
	return vertVector[i];
}
