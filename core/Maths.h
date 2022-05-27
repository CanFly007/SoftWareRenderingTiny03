#ifndef MATHS_H
#define MATHS_H

#include <cmath>
#include <iostream>

#include "./macro.h"

class Vec3
{
public:
	Vec3();
	Vec3(float e0, float e1, float e2);
	float& operator[](int index);
	Vec3 operator-()const;
	Vec3 operator-(const Vec3& v)const; //减法
	Vec3 operator+(const Vec3& v)const;
	Vec3& operator+=(const Vec3& v);
	Vec3 operator*(const float t)const;
	Vec3 operator/(const float t)const; //除法
	float operator*(const Vec3& v)const;//点积
	float length()const;
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
	float operator[](int index)const;
	Vec2 operator+(const Vec2& v)const;
	Vec2 operator-(const Vec2& v)const;//减法
	Vec2 operator*(const float t)const;//乘法

public:
	union {//调用构造函数会初始化成{0,0}
		struct { float x, y; };
		float e[2];
	};
};

class Vec4
{
public:
	Vec4();
	Vec4(float e0, float e1, float e2, float e3);
	Vec4(Vec3 v3, float alpha);
	float operator[](int index)const;
	float& operator[](int index);//float& 看下是否报错，将a[]设置为左值，报错！float是一个立即数不能成为左值
	Vec4 operator*(const float t)const;
	Vec4 operator/(const float t)const;

public:
	union {
		struct { float x, y, z, w; };
		float e[4];
	};
};

class Mat3
{
public:
	Mat3();
	Vec3 operator[](int i)const;
	Vec3& operator[](int i);
	Mat3 operator/(float t)const;
	Vec3 operator*(Vec3 v3)const;
	Mat3 transpose() const;
	Mat3 inverse() const;
	//Mat3 inverse_transpose() const;
	static Mat3 identity();

public:
	Vec3 rows[3];
};

class Mat4
{
public:
	Mat4();
	Vec4& operator[](int i);
	Vec4 operator[](int i) const;
	Mat4 operator/(float t) const;
	Mat4 operator*(const Mat4 B) const;
	Vec4 operator*(const Vec4 V) const;

	Mat4 transpose() const;
	Mat4 inverse() const;
	//Mat4 inverse_transpose() const;
	static Mat4 identity();
public:
	Vec4 rows[4];
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
	float length = std::sqrt(v * v);
	return v / length;

	//float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	//return Vec3(v.x / length, v.y / length, v.z / length);
}

Vec3 Convert_ToVec3(const Vec4& v4);
Vec3 operator*(float t, const Vec3& v);

Mat4 WorldToViewMat(Vec3 cameraPos, Vec3 lookAtPos, Vec3 upDir);
Mat4 OrthoProjection(float cameraWidth, float cameraHeight, float near, float far);
#endif // !MATHS_H
