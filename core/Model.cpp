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
		if (line.compare(0, 2, "v ") == 0)//������0��ʼ������2���ַ�������0��ʾ��ͬ
		{
			iss >> trash;
			Vec3 v;
			for (int i = 0; i < 3; i++)
				iss >> v[i];
			vertVector.push_back(v);//v������Ԫ�أ�verts��ÿһ��Ԫ����3������ÿ��Ԫ�ر�ʾ.obj�е�һ��
		}
		else if (line.compare(0, 3, "vt ") == 0)
		{
			iss >> trash >> trash;//�ֱ���v��t�ַ�
			Vec2 vt;
			for (int i = 0; i < 2; i++)//.obj�ļ���vt�����ж���ո�û��ϵ
				iss >> vt[i];
			uvVector.push_back(vt);//uvs��ʾÿһ��vt�ļ���
		}
		else if (line.compare(0, 3, "vn ") == 0)
		{
			iss >> trash >> trash;
			Vec3 vn;
			for (int i = 0; i < 3; i++)
				iss >> vn[i];//raw[i]����vn.x/vn.y��vn.z
			normalVector.push_back(vn);//������normal�ļ��ϣ�֮����������normals�����в���
		}
		else if (line.compare(0, 2, "f ") == 0)
		{
			//f 24/1/24 25/2/25 26/3/26
			std::vector<int> f;
			int indexVertex, indexUV, indexNormal;
			std::vector<int> faceUV;//����������������ɣ���Ӧ����������������uvֵ
			iss >> trash;//trash = f
			//indexVertex=24 trash=/ indexUV=1 trash=/ indexNormal=24��f��ÿһ��ִ������ѭ��
			while (iss >> indexVertex >> trash >> indexUV >> trash >> indexNormal)
			{
				indexVertex--; // in wavefront obj all indices start at 1, not zero
				f.push_back(indexVertex);//1��Ӧ����vector<Vec3f> verts_�����е�0����
				indexUV--;//����Ҳ�Ǵ�1��ʼ�ۼƵģ��ŵ������д�0��ʼ
				f.push_back(indexUV);//ÿ��f�м������������uv����������f������Ԫ��
				indexNormal--;
				f.push_back(indexNormal);
			}
			faceVector.push_back(f);//f��9��Ԫ�أ����һ���棬faces�����е��漯��,faces��ÿ��Ԫ�ؼ�.obj��ÿһ��
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
		texfile = texfile.substr(0, dot) + std::string(suffix);//filenameȥ����׺.obj + suffix����
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
	std::vector<int> face = faceVector[iface];//f��һ��
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