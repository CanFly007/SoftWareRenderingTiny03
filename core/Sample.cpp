#include "Sample.h"

static int CalCubemapUV(Vec3 direction, Vec2& uv)//哪个绝对值大就在哪边，其他两个分量都除以最大分量的绝对值就得到范围[-1,1]
{
	float absX = fabs(direction.x);
	float absY = fabs(direction.y);
	float absZ = fabs(direction.z);

	if (absX > absY && absX > absZ)
	{
		if (direction.x > 0)
		{
			uv.u = (direction.z / absX + 1.0f) / 2.0f;
			uv.v = (direction.y / absX + 1.0f) / 2.0f;
			return 0;
		}
		else
		{
			uv.u = (-direction.z / absX + 1.0f) / 2.0f;
			uv.v = (direction.y / absX + 1.0f) / 2.0f;
			return 1;
		}
	}
	else if (absY > absZ)
	{
		if (direction.y > 0)
		{
			uv.u = (direction.x / absY + 1.0f) / 2.0f;
			uv.v = (direction.z / absY + 1.0f) / 2.0f;
			return 2;
		}
		else
		{
			uv.u = (direction.x / absY + 1.0f) / 2.0f;
			uv.v = (-direction.z / absY + 1.0f) / 2.0f;
			return 3;
		}
	}
	else
	{
		if (direction.z > 0)
		{
			uv.u = (-direction.x / absZ + 1.0f) / 2.0f;
			uv.v = (direction.y / absZ + 1.0f) / 2.0f;
			return 4;
		}
		else
		{
			uv.u = (direction.x / absZ + 1.0f) / 2.0f;
			uv.v = (direction.y / absZ + 1.0f) / 2.0f;
			return 5;
		}
	}
}

Vec3 texture_sample(Vec2 uv, TGAImage* image)
{
	if (image == nullptr)//没有这张图，返回黑色
		return Vec3(0, 0, 0);

	uv[0] = fmod(uv[0], 1);
	uv[1] = fmod(uv[1], 1);//求余运算 表示clamp
	//printf("%f %f\n", uv[0], uv[1]);
	int uv0 = uv[0] * image->get_width();
	int uv1 = uv[1] * image->get_height();
	TGAColor c = image->get(uv0, uv1);//c的范围是[0,255]
	Vec3 res;
	for (int i = 0; i < 3; i++)
		res[2 - i] = (float)c[i] / 255.f; //应该就错在这块，TGAColor类顺序是b g r a，要结果res把前三项颠倒一下
	return res;//返回范围是[0.0, 1.0]
}

Vec3 cubemap_sample(Vec3 direction, cubemap_t* cubemap)
{
	Vec2 uv;//out参数
	int face_index = CalCubemapUV(direction, uv);//返回在哪个面采样，out uv返回这个面的采样uv是多少
	return texture_sample(uv, cubemap->faces[face_index]);
}