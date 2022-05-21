#include "./Maths.h"

Vec2::Vec2() :e{ 0,0 } {}
Vec2::Vec2(float e0, float e1) : e{ e0,e1 } {}
Vec2::Vec2(Vec3 v3) : e{ v3.x,v3.y } {}
float& Vec2::operator[](int index) { return e[index]; }
Vec2 Vec2::operator+(const Vec2& v)const { return Vec2(x + v.x, y + v.y); }
Vec2 Vec2::operator-(const Vec2& v)const { return Vec2(x - v.x, y - v.y); }
Vec2 Vec2::operator*(const float t)const { return Vec2(x * t, y * t); }

Vec3::Vec3() : e{ 0,0,0 } {}
Vec3::Vec3(float e0, float e1, float e2) : e{ e0,e1,e2 } {}
float& Vec3::operator[](int index) { return e[index]; }
Vec3 Vec3::operator-(const Vec3& v)const { return Vec3(x - v.x, y - v.y, z - v.z); }
Vec3 Vec3::operator/(const float t)const { return Vec3(x / t, y / t, z / t); }
float Vec3::operator*(const Vec3& v)const { return x * v.x + y * v.y + z * v.z; }
