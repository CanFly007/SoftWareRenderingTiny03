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

	//���ԣ�Ϊelfgirl�ṩ����
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
	if (_access(texfile.data(), 0) != -1)//windows API ������������ļ�
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
		texfile = texfile.substr(0, dot) + std::string(suffix);//filenameȥ����׺.obj + suffix����
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
	uv.x = fmod(uv.x, 1); uv.y = fmod(uv.y, 1);//��������
	int diffuseMapW = diffuseMap->get_width();
	int diffuseMapH = diffuseMap->get_height();
	TGAColor color = diffuseMap->get((int)(diffuseMapW * uv.x), (int)(diffuseMapH * uv.y));//[0,255]

	Vec3 result;
	for (int i = 0; i < 3; i++)
		result[2 - i] = (float)color[i] / 255.0;//b, g, r, a˳��
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
