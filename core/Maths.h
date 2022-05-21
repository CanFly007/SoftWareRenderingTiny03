#ifndef MATHS_H
#define MATHS_H
class Vec3
{
public:
	Vec3();
	Vec3(float e0, float e1, float e2);
	float& operator[](int index);
public:
	union {//���ù��캯�����ʼ����{0,0,0}
		struct { float x, y, z; };
		float e[3];
	};
};

class Vec2
{
public:
	Vec2();
	Vec2(float e0, float e1);
	Vec2(Vec3 v3);
	float& operator[](int index);
	Vec2 operator-(const Vec2& v)const;

public:
	union {//���ù��캯�����ʼ����{0,0}
		struct { float x, y; };
		float e[2];
	};
};

//ȫ�ֺ���
inline float Cross(Vec2 v1, Vec2 v2) { return v1.x * v2.y - v1.y * v2.x; }
#endif // !MATHS_H
