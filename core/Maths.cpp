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

Vec4::Vec4() :e{ 0,0,0,0 } {}
Vec4::Vec4(float e0, float e1, float e2, float e3) : e{ e0,e1,e2,e3 } {}
float Vec4::operator[](int index)const { return e[index]; }
float& Vec4::operator[](int index) { return e[index]; }
Vec4 Vec4::operator*(const float t)const { return Vec4(x * t, y * t, z * t, w * t); }
Vec4 Vec4::operator/(const float t)const { return Vec4(x / t, y / t, z / t, w / t); }

Mat3::Mat3() {}
Vec3 Mat3::operator[](int i)const { return rows[i]; }//右值，const的
Vec3& Mat3::operator[](int i) { return rows[i]; }//左值，可赋值的
Mat3 Mat3::transpose() const{
	Mat3 transpose;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			transpose[i][j] = (*this)[j][i];
	return transpose;
}
//Mat3 Mat3::inverse() const {
//
//}
//Mat3 Mat3::inverse_transpose() const;
Mat3 Mat3::identity() {
	Mat3 mat;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			mat[i][j] = (i == j);
	return mat;
}

Mat4::Mat4() {}
Vec4& Mat4::operator[](int i) { return rows[i]; }
Vec4 Mat4::operator[](int i) const { return rows[i]; }

Mat4 Mat4::transpose() const {
	Mat4 mat;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			mat[i][j] = (*this)[j][i];
	return mat;
}
//Mat4 inverse() const;
//Mat4 inverse_transpose() const;
Mat4 Mat4::identity() {
	Mat4 mat;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			mat[i][j] = (i == j);
	return mat;
}



std::ostream& operator<<(std::ostream& out, const Vec2& v)
{
	return out << v.e[0] << ' ' << v.e[1];
}
std::ostream& operator<<(std::ostream& out, const Vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
std::ostream& operator<<(std::ostream& out, const Vec4& v)
{
	return out << v[0] << " " << v[1] << " " << v[2] << " " << v[3];
}
std::ostream& operator<<(std::ostream& out, const Mat3& m)
{
	return out << m[0] << "\n" << m[1] << "\n" << m[2];
}
std::ostream& operator<<(std::ostream& out, const Mat4& m)
{
	return out << m[0] << "\n" << m[1] << "\n" << m[2] << "\n" << m[3];
}
