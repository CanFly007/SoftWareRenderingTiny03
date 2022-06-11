#include "Sample.h"

static int CalCubemapUV(Vec3 direction, Vec2& uv)//�ĸ�����ֵ������ıߣ��������������������������ľ���ֵ�͵õ���Χ[-1,1]
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
	if (image == nullptr)//û������ͼ�����غ�ɫ
		return Vec3(0, 0, 0);

	uv[0] = fmod(uv[0], 1);
	uv[1] = fmod(uv[1], 1);//�������� ��ʾclamp
	//printf("%f %f\n", uv[0], uv[1]);
	int uv0 = uv[0] * image->get_width();
	int uv1 = uv[1] * image->get_height();
	TGAColor c = image->get(uv0, uv1);//c�ķ�Χ��[0,255]
	Vec3 res;
	for (int i = 0; i < 3; i++)
		res[2 - i] = (float)c[i] / 255.f; //Ӧ�þʹ�����飬TGAColor��˳����b g r a��Ҫ���res��ǰ����ߵ�һ��
	return res;//���ط�Χ��[0.0, 1.0]
}

Vec3 cubemap_sample(Vec3 direction, cubemap_t* cubemap)
{
	Vec2 uv;//out����
	int face_index = CalCubemapUV(direction, uv);//�������ĸ��������out uv���������Ĳ���uv�Ƕ���
	return texture_sample(uv, cubemap->faces[face_index]);
}