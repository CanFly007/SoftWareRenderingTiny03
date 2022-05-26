#ifndef SHADER_H
#define SHADER_H

#include "../core/Maths.h"
#include "../core/Model.h"

typedef struct
{
	Model* model;//.obj�ļ�
	Mat4 mvp_mat4;

	Vec2 uv_attri[3];//���������uv����
	Vec4 orthoSpace_Pos[3];//�������㾭��MVP�任���ڲü��ռ������
	
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
