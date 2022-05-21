#ifndef MATHS_H
#define MATHS_H

#include <cmath>
#include <iostream>

class Vec3
{
public:
	Vec3();
	Vec3(float e0, float e1, float e2);
	float& operator[](int index);
	Vec3 operator-(const Vec3& v)const; //减法
	Vec3 operator/(const float t)const; //除法
	float operator*(const Vec3& v)const;//点积
public:
	union {//调用构造函数会初始化成{0,0,0}
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
	Vec2 operator+(const Vec2& v)const;
	Vec2 operator-(const Vec2& v)const;//减法
	Vec2 operator*(const float t)const;//乘法

public:
	union {//调用构造函数会初始化成{0,0}
		struct { float x, y; };
		float e[2];
	};
};

//全局函数
inline float Cross(Vec2 v1, Vec2 v2) { return v1.x * v2.y - v1.y * v2.x; }
inline Vec3 Cross(Vec3 v1, Vec3 v2)
{
	return Vec3(v1[1] * v2[2] - v1[2] * v2[1],
				v1[2] * v2[0] - v1[0] * v2[2],
				v1[0] * v2[1] - v1[1] * v2[0]);
}



inline Vec3 normalize(const Vec3 v)
{
	float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	return Vec3(v.x / length, v.y / length, v.z / length);
	//return (v / length);
}
#endif // !MATHS_H
