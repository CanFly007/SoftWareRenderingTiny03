#include "Sample.h"

Vec3 texture_sample(Vec2 uv, TGAImage* image)
{
	uv[0] = fmod(uv[0], 1);
	uv[1] = fmod(uv[1], 1);
	//printf("%f %f\n", uv[0], uv[1]);
	int uv0 = uv[0] * image->get_width();
	int uv1 = uv[1] * image->get_height();
	TGAColor c = image->get(uv0, uv1);
	Vec3 res;
	for (int i = 0; i < 3; i++)
		res[2 - i] = (float)c[i] / 255.f; //Ӧ�þʹ�����飬TGAColor��˳����b g r a��Ҫ���res��ǰ����ߵ�һ��
	return res;
}