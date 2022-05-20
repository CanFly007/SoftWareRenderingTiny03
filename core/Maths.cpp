#include "./Maths.h"

Vec2::Vec2() :e{ 0,0 } {}
Vec2::Vec2(float e0, float e1) : e{ e0,e1 } {}
float& Vec2::operator[](int index) { return e[index]; }

Vec3::Vec3() : e{ 0,0,0 } {}
Vec3::Vec3(float e0, float e1, float e2) : e{ e0,e1,e2 } {}
float& Vec3::operator[](int index) { return e[index]; }
