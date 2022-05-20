#ifndef MATHS_H
#define MATHS_H

class Vec2
{
public:
	Vec2();
	Vec2(float e0, float e1);
	float& operator[](int index);
public:
	union {//调用构造函数会初始化成{0,0}
		struct { float x, y; };
		float e[2];
	};
};

class Vec3
{
public:
	Vec3();
	Vec3(float e0, float e1, float e2);
	float& operator[](int index);
public:
	union {//调用构造函数会初始化成{0,0,0}
		struct { float x, y, z; };
		float e[3];
	};
};
#endif // !MATHS_H
