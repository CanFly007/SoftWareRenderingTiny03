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
}

Model::~Model()
{

}

int Model::nfaces()
{
	return (int)faceVector.size();
}

Vec3 Model::GetVertPos(int iface, int nvert)
{
	std::vector<int> face = faceVector[iface];//f��һ��
	int index = face[nvert * 3];//0 3 6
	Vec3 vertPos = vertVector[index];
	return vertPos;
}
