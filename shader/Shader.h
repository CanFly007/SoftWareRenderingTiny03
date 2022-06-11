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
	cubemap_t* irradianceMap;//漫反射IBL
	TGAImage* brdfLut;
	cubemap_t* prefilter_maps[15];
	
}IBLMap_t;

typedef struct //attribute顶点属性，uniform顶点片元都能用cpu传进来的，varying顶点到片元
{
	//Attribute:
	Model* model;//.obj文件
	Camera* camera;
	Vec2 uv_attribute[3];//三个顶点的uv坐标
	Vec3 normal_attribute[3];

	//uniform:
	Mat4 MVP_uniform;

	//varying:
	Vec3 worldSpacePos_varying[3];
	Vec4 clipSpacePos_varying[3];//正常情况：三个顶点经过MVP变换后在裁剪空间的坐标。被齐次裁剪情况：带有与平面的交点的一个三角形

	//用于比如X齐次裁剪平面，裁剪后的顶点位置数组
	Vec4 outClipSpacePos[MAX_VERTEX];//因为不知道裁剪后是几个顶点，所以用MAX_VERTEX表示
	Vec3 outWorldSpacePos[MAX_VERTEX];
	
	//IBL
	IBLMap_t* iblMap;
}payload_t;//理解成a2v和v2f的并集合

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
