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
		return;
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

	create_map(filename);
}

void Model::create_map(const char* filename)
{
	load_texture(filename, "_diffuse.tga", diffuseMap);
}

void Model::load_texture(std::string filename, const char* suffix, TGAImage& img)
{
	std::string texfile(filename);
	size_t dot = texfile.find_last_of(".");
	if (dot != std::string::npos)
	{
		texfile = texfile.substr(0, dot) + std::string(suffix);//filename去掉后缀.obj + suffix名称
		std::cerr << "texture file " << texfile << " loading " << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
		img.flip_vertically();
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
TGAColor Model::SamplerDiffseColor(Vec2 uv)
{
	int diffuseMapW = diffuseMap.get_width();
	int diffuseMapH = diffuseMap.get_height();
	return diffuseMap.get((int)(diffuseMapW * uv.x), (int)(diffuseMapH * uv.y));
}




std::vector<int> Model::face(int idx)
{
	return faceVector[idx];
}
Vec3 Model::vert(int i)
{
	return vertVector[i];
}




//Old
//#include <iostream>
//#include <string>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include "model.h"
//
//Model::Model(const char* filename) :verts_(), faces_()
//{
//	std::ifstream in;
//	in.open(filename, std::ifstream::in);
//	if (in.fail())return;
//	std::string line;
//	while (!in.eof())
//	{
//		std::getline(in, line);
//		std::istringstream iss(line.c_str());
//		char trash;
//		if (!line.compare(0, 2, "v "))
//		{
//			iss >> trash;
//			Vec3 v;
//			for (int i = 0; i < 3; i++)
//				iss >> v[i];
//			verts_.push_back(v);
//		}
//		else if (!line.compare(0, 3, "vt "))
//		{
//			iss >> trash >> trash;
//			Vec2 uv;
//			iss >> uv.x;
//			iss >> uv.y;
//			//for (int i = 0; i < 2; i++)
//			//	iss >> uv[i];
//			uv_.push_back(uv);
//		}
//		else if (!line.compare(0, 2, "f "))
//		{
//			std::vector<int> f;
//			int itrash, idx;
//			iss >> trash;
//			while (iss >> idx >> trash >> itrash >> trash >> itrash)
//			{
//				idx--;
//				f.push_back(idx);
//			}
//			faces_.push_back(f);
//		}
//	}
//	std::cerr << "# v# " << verts_.size() << " f# " << faces_.size() << " vt# " << uv_.size() << std::endl;
//	load_texture(filename, "_diffuse.tga", diffusemap_);
//}
//
//Model::~Model() {}
//
//int Model::nverts()
//{
//	return (int)verts_.size();
//}
//int Model::nfaces()
//{
//	return (int)faces_.size();
//}
//std::vector<int> Model::face(int idx)
//{
//	return faces_[idx];
//}
//Vec3 Model::vert(int i)
//{
//	return verts_[i];
//}
//
//void Model::load_texture(std::string filename, const char* suffix, TGAImage& img)
//{
//	std::string texfile(filename);
//	size_t dot = texfile.find_last_of(".");
//	if (dot != std::string::npos)
//	{
//		texfile = texfile.substr(0, dot) + std::string(suffix);
//		std::cerr << "texture file " << texfile << " loading" << (img.read_tga_file(texfile.c_str()) ? "ok" : "failed") << std::endl;
//		img.flip_vertically();
//	}
//}
//
//TGAColor Model::diffuse(Vec2 uv)
//{
//	return diffusemap_.get(uv.x, uv.y);
//}
//
//Vec2 Model::uv(int iface, int nvert)
//{
//	std::vector<int> face = faces_[iface];
//	int idx = face[nvert];
//	//int idx = faces_[iface][nvert][1];
//	return Vec2(uv_[idx].x * diffusemap_.get_width(), uv_[idx].y * diffusemap_.get_height());
//}