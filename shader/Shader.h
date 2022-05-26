#ifndef SHADER_H
#define SHADER_H

#include "../core/Maths.h"
#include "../core/Model.h"

typedef struct
{
	Model* model;//.obj文件
	Mat4 mvp_mat4;

	Vec2 uv_attri[3];//三个顶点的uv坐标
	Vec4 orthoSpace_Pos[3];//三个顶点经过MVP变换后在裁剪空间的坐标
	
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

#endif // !SHADER_H
