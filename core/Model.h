#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Maths.h"
#include "../tgaimage.h"

class Model
{
private:
	std::vector<Vec3> vertVector;
	std::vector<std::vector<int>> faceVector;//vertex/uv/normal.�����vector�����9��Ԫ�أ���f��һ�У��������vector������9��Ԫ�صļ���
	std::vector<Vec3> normalVector;
	std::vector<Vec2> uvVector;

	TGAImage* diffuseMap;

public:
	Model(const char* filename);
	~Model();
	int nfaces();//���ض��ٸ���������f �ж�����
	Vec3 GetVertPos(int iface, int nthvert);//���ص�iface���е�nvert����Positon
	void create_map(const char* filename);
	void load_texture(std::string filename, const char* suffix, TGAImage* img);
	Vec3 SamplerDiffseColor(Vec2 uv);
	Vec2 GetVertUV(int iface, int nthvert);

	std::vector<int> face(int idx);
		Vec3 vert(int i);

};
#endif // !MODEL_H



//Old
//#ifndef __MODEL_H__
//#define __MODEL_H__
//
//#include<vector>
//#include "Maths.h"
//#include "../tgaimage.h"
//
//class Model
//{
//private:
//	std::vector<Vec3> verts_;
//	std::vector<std::vector<int>> faces_;
//	std::vector<Vec2> uv_;
//	TGAImage diffusemap_;
//	void load_texture(std::string filename, const char* suffix, TGAImage& img);
//public:
//	Model(const char* filename);
//	~Model();
//	int nverts();
//	int nfaces();
//	Vec3 vert(int i);
//	Vec2 uv(int iface, int nvert);
//	TGAColor diffuse(Vec2 uv);
//	std::vector<int> face(int idx);
//};
//
//#endif