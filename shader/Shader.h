#ifndef SHADER_H
#define SHADER_H

#include "../core/macro.h"
#include "../core/Maths.h"
#include "../core/Model.h"
#include "../core/Camera.h"

typedef struct cubemap
{
	TGAImage* faces[6];
}cubemap_t;

typedef struct IBLMap
{
	int mipMapLevels;
	cubemap_t* irradianceMap;//������IBL
	TGAImage* brdfLut;
	cubemap_t* prefilter_maps[15];
	
}IBLMap_t;

typedef struct //attribute�������ԣ�uniform����ƬԪ������cpu�������ģ�varying���㵽ƬԪ
{
	//Attribute:
	Model* model;//.obj�ļ�
	Camera* camera;
	Vec2 uv_attribute[3];//���������uv����
	Vec3 normal_attribute[3];

	//uniform:
	Mat4 MVP_uniform;

	//varying:
	Vec3 worldSpacePos_varying[3];
	Vec4 clipSpacePos_varying[3];//����������������㾭��MVP�任���ڲü��ռ�����ꡣ����βü������������ƽ��Ľ����һ��������

	//���ڱ���X��βü�ƽ�棬�ü���Ķ���λ������
	Vec4 outClipSpacePos[MAX_VERTEX];//��Ϊ��֪���ü����Ǽ������㣬������MAX_VERTEX��ʾ
	Vec3 outWorldSpacePos[MAX_VERTEX];
	
	//IBL
	IBLMap_t* iblMap;
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

class SkyboxShader :public IShader
{
public:
	void vertex_shader(int nfaces, int nvertex);
	Vec3 fragment_shader(float alpha, float beta, float gamma);
};

class PBRShader :public IShader
{
public:
	void vertex_shader(int nfaces, int nvertex);
	Vec3 fragment_shader(float alpha, float beta, float gamma);
};

#endif // !SHADER_H
