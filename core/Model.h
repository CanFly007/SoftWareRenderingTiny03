#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Maths.h"

class Model
{
private:
	std::vector<Vec3> vertVector;
	std::vector<std::vector<int>> faceVector;//vertex/uv/normal.�����vector�����9��Ԫ�أ���f��һ�У��������vector������9��Ԫ�صļ���
	std::vector<Vec3> normalVector;
	std::vector<Vec2> uvVector;

public:
	Model(const char* filename);
	~Model();
	int nfaces();//���ض��ٸ���������f �ж�����
	Vec3 GetVertPos(int iface, int nvert);//���ص�iface���е�nvert����Positon
};
#endif // !MODEL_H
