#ifndef SHADER_H
#define SHADER_H

#include "../core/Maths.h"
#include "../core/Model.h"

typedef struct //attribute�������ԣ�uniform����ƬԪ������cpu�������ģ�varying���㵽ƬԪ
{
	//Attribute:
	Model* model;//.obj�ļ�
	Vec2 uv_attribute[3];//���������uv����
	Vec3 normal_attribute[3];

	//uniform:
	Mat4 MVP_uniform;

	//varying:
	Vec3 worldSpacePos_varying[3];
	Vec4 clipSpacePos_varying[3];//�������㾭��MVP�任���ڲü��ռ������
	
}payload_t;//����a2v��v2f�Ĳ�����

class IShader
{
public:
	payload_t payload;
	virtual void vertex_shader(int nfaces, int nvertex) {}
	virtual Vec3 fragment_shader(float alpha, float beta, float gamma) { return Vec3(0, 0, 0); }
};

class PhongShader :public IShader
{
public:
	void vertex_shader(int nfaces, int nvertex);
	Vec3 fragment_shader(float alpha, float beta, float gamma);

};

#endif // !SHADER_H
