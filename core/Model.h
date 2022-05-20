#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "Maths.h"

class Model
{
private:
	std::vector<Vec3> vertVector;
	std::vector<std::vector<int>> faceVector;//vertex/uv/normal.里面的vector存的是9个元素（即f中一行），外面的vector是所有9个元素的集合
	std::vector<Vec3> normalVector;
	std::vector<Vec2> uvVector;

public:
	Model(const char* filename);
	~Model();
	int nfaces();//返回多少个面数，即f 有多少行
	Vec3 GetVertPos(int iface, int nvert);//返回第iface面中第nvert顶点Positon
};
#endif // !MODEL_H
