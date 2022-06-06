#include "./Maths.h"

Vec2::Vec2() :e{ 0,0 } {}
Vec2::Vec2(float e0, float e1) : e{ e0,e1 } {}
Vec2::Vec2(Vec3 v3) : e{ v3.x,v3.y } {}
float& Vec2::operator[](int index) { return e[index]; }
float Vec2::operator[](int index)const { return e[index]; }
Vec2 Vec2::operator+(const Vec2& v)const { return Vec2(x + v.x, y + v.y); }
Vec2 Vec2::operator-(const Vec2& v)const { return Vec2(x - v.x, y - v.y); }
Vec2 Vec2::operator*(const float t)const { return Vec2(x * t, y * t); }
Vec2 Vec2::operator/(const float t)const { return Vec2(x / t, y / t); }

Vec3::Vec3() : e{ 0,0,0 } {}
Vec3::Vec3(float e0, float e1, float e2) : e{ e0,e1,e2 } {}
float& Vec3::operator[](int index) { return e[index]; }
Vec3 Vec3::operator-()const { return Vec3(-x, -y, -z); }
Vec3& Vec3::operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
Vec3 Vec3::operator+(const Vec3& v)const { return Vec3(x + v.x, y + v.y, z + v.z); }
Vec3 Vec3::operator-(const Vec3& v)const { return Vec3(x - v.x, y - v.y, z - v.z); }
Vec3 Vec3::operator-(const float t)const { return Vec3(x - t, y - t, z - t); }
Vec3 Vec3::operator*(const float t)const { return Vec3(x * t, y * t, z * t); }
Vec3 Vec3::operator/(const float t)const { return Vec3(x / t, y / t, z / t); }
float Vec3::operator*(const Vec3& v)const { return x * v.x + y * v.y + z * v.z; }
float Vec3::length()const { return sqrt(x * x + y * y + z * z); }

Vec4::Vec4() :e{ 0,0,0,0 } {}
Vec4::Vec4(float e0, float e1, float e2, float e3) : e{ e0,e1,e2,e3 } {}
Vec4::Vec4(Vec3 v3, float alpha) : e{ v3.x,v3.y,v3.z,alpha } {}
float Vec4::operator[](int index)const { return e[index]; }
float& Vec4::operator[](int index) { return e[index]; }
Vec4 Vec4::operator+(const Vec4& v)const { return Vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
Vec4 Vec4::operator-(const Vec4& v)const { return Vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
Vec4 Vec4::operator*(const float t)const { return Vec4(x * t, y * t, z * t, w * t); }
Vec4 Vec4::operator/(const float t)const { return Vec4(x / t, y / t, z / t, w / t); }

Mat3::Mat3() {}
Vec3 Mat3::operator[](int i)const { return rows[i]; }//��ֵ��const��
Vec3& Mat3::operator[](int i) { return rows[i]; }//��ֵ���ɸ�ֵ��
Mat3 Mat3::operator/(float t)const
{
	Mat3 m;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m[i][j] = (*this)[i][j] / t;
	return m;
}
Vec3 Mat3::operator*(Vec3 v3)const
{
	Vec3 result;
	for (int i = 0; i < 3; i++)
		for (int k = 0; k < 3; k++)
			result[i] += (*this)[i][k] * v3[k];
	return result;
}
Mat3 Mat3::transpose() const{
	Mat3 transpose;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			transpose[i][j] = (*this)[j][i];
	return transpose;
}
//����ʽ��ֵ����������һ�У�һ�У��ĸ�Ԫ�س�������Ӧ�Ĵ�������ʽ�˻�֮��
static float Mat3_determinant(const Mat3& m)
{
	float a = +m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
	float b = -m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]);
	float c = +m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
	return a + b + c;
}
//������󣺴�������ʽ�������򣨼������������ת��һ�Σ�
static Mat3 Mat3_adjoint(const Mat3& m)
{
	Mat3 adjoint;
	float A00 = +(m[1][1] * m[2][2] - m[1][2] * m[2][1]);
	float A01 = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]);
	float A02 = +(m[1][0] * m[2][1] - m[1][1] * m[2][0]);
	float A10 = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]);
	float A11 = +(m[0][0] * m[2][2] - m[0][2] * m[2][0]);
	float A12 = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]);
	float A20 = +(m[0][1] * m[1][2] - m[0][2] * m[1][1]);
	float A21 = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]);
	float A22 = +(m[0][0] * m[1][1] - m[0][1] * m[1][0]);
	adjoint[0][0] = A00;
	adjoint[0][1] = A01;
	adjoint[0][2] = A02;
	adjoint[1][0] = A10;
	adjoint[1][1] = A11;
	adjoint[1][2] = A12;
	adjoint[2][0] = A20;
	adjoint[2][1] = A21;
	adjoint[2][2] = A22;
	adjoint = adjoint.transpose();//����������ǰ����������ת��һ��	return adjoint;
	return adjoint;
}
//����󣺰��������Ծ�������ʽ��ֵ
Mat3 Mat3::inverse() const {
	float determinant = Mat3_determinant(*this);
	Mat3 adjoint = Mat3_adjoint(*this);
	Mat3 inverse = adjoint / determinant;
	return inverse;
}
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
Mat4 Mat4::operator/(float t) const {
	Mat4 m;
	for (int i = 0; i < 4; i++)
		m[i] = rows[i] / t;
	return m;
}
Mat4 Mat4::operator*(const Mat4 B) const {
	Mat4 m;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				m[i][j] += (*this)[i][k] * B[k][j];
	return m;
}
Vec4 Mat4::operator*(const Vec4 V) const
{
	Vec4 result;
	for (int i = 0; i < 4; i++)
		for (int k = 0; k < 4; k++)
			result[i] += (*this)[i][k] * V[k];
	return result;
}
Mat4 Mat4::transpose() const {
	Mat4 mat;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			mat[i][j] = (*this)[j][i];
	return mat;
}
//����r��c���ϴ�������ʽ��ֵ
//��������ʽ��ȥ����r�к͵�c�У����µ�3x3�����ɵ�����ʽ��Ϊ����ʽ��ǰ����������ż��Ǵ�������ʽ
static float Mat4_algebraicComplement(const Mat4& m4, int r, int c)
{
	Mat3 complement;//����ʽ��ʽ�ľ���
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int row = i < r ? i : i + 1;
			int col = j < c ? j : j + 1;
			complement[i][j] = m4[row][col];
		}
	}
	float determinant = Mat3_determinant(complement);//����ʽ��ֵ
	int sign = (r + c) % 2 == 0 ? 1 : -1;
	return sign * determinant;
}
//����ʽ��ֵ������һ�У���һ�У�����Ԫ�س�������Ӧ�Ĵ�������ʽ�˻�֮��
static float Mat4_determinant(const Mat4& m)
{
	float determinant = 0;
	for (int j = 0; j < 4; j++)//����0�и���Ԫ�س˻�֮��
		determinant += m[0][j] * Mat4_algebraicComplement(m, 0, j);
	return determinant;
}
//������󣺴�������ʽ�������򣨼������������ת��һ�Σ�
static Mat4 Mat4_adjoint(const Mat4& m)
{
	Mat4 adjoint;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			adjoint[i][j] = Mat4_algebraicComplement(m, i, j);
	adjoint = adjoint.transpose();
	return adjoint;
}
//����󣺰��������Ծ�������ʽ��ֵ
Mat4 Mat4::inverse() const {
	Mat4 inverse;
	Mat4 adjoint = Mat4_adjoint(*this);
	float determinant = Mat4_determinant(*this);
	inverse = adjoint / determinant;
	return inverse;
}
//Mat4 inverse_transpose() const;
Mat4 Mat4::identity() {
	Mat4 mat;
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			mat[i][j] = (i == j);
	return mat;
}

Vec3 Convert_ToVec3(const Vec4& v4)
{
	return Vec3(v4.x, v4.y, v4.z);
}
Vec3 operator*(float t, const Vec3& v)
{
	return Vec3(t * v.x, t * v.y, t * v.z);
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

Mat4 WorldToViewMat(Vec3 cameraPos, Vec3 lookAtPos, Vec3 upDir)
{
	Vec3 z = normalize(cameraPos - lookAtPos);
	Vec3 x = normalize(Cross(upDir, z));
	Vec3 y = normalize(Cross(z, x));
	Mat4 viewMat = Mat4::identity();
	for (int j = 0; j < 3; j++)
	{
		viewMat[0][j] = x[j];//��������
		viewMat[1][j] = y[j];
		viewMat[2][j] = z[j];
	}
	//�ȷ���ƽ��(-cameraPos�ŵ�������)������ת�����Ե������ǣ��������ÿһ�к͵����е��
	viewMat[0][3] = x * (-cameraPos);
	viewMat[1][3] = y * (-cameraPos);
	viewMat[2][3] = z * (-cameraPos);
	return viewMat;
}
Mat4 OrthoProjection(float cameraWidth, float cameraHeight, float near, float far)
{
	Mat4 ortho = Mat4::identity();
	ortho[0][0] = 2.0 / cameraWidth;
	ortho[1][1] = 2.0 / cameraHeight;
	ortho[2][2] = 2.0 / (near - far);//����������ת����
	ortho[2][3] = (near + far) / (near - far);//z����ƽ�Ƶ�ԭ��
	return ortho;
}
 //*           2*near/width              0             0           0
 //*                      0  2*near/height             0           0
 //*                      0              0    (n+f)/(n-f)   2fn/(n-f)
 //*                      0              0            -1           0
Mat4 PerspectiveProjection(float FOV, float aspect, float near, float far)
{
	//����FOV�Ϳ�߱ȣ������ͳ�
	float halfFOV = FOV / 2;//FOV�������ĽǶ�
	halfFOV = halfFOV / 180.0 * PI;//ת���ɻ���
	float halfHeight = tan(halfFOV) * near;
	float height = halfHeight * 2.0;
	float width = aspect * height;

	Mat4 perspective = Mat4::identity();
	//ViewSpace��w������1��ClipSpace��w������-Z������͸�ӱ任����ĵ����е�������-1
	//ViewSpace��x��y������ͨ������������ͶӰ��������Ľ�ƽ�棬Ȼ���[-width,width]ѹ�⵽[-1,1]��ע��Ҫ�������-Z����Ϊ͸�ӳ���ClipSpace->NDC)
	//ViewSpace��z����������������ΪA,��������ΪB���ֱ�(-n,-1)��(-f,1)����Zn = (A * Ze + B)/(-Ze)�У����A��B��ֵ
	perspective[0][0] = 2 * near / width;
	perspective[1][1] = 2 * near / height;
	perspective[2][2] = (near + far) / (near - far);
	perspective[2][3] = 2 * far * near / (near - far);
	perspective[3][2] = -1;
	return perspective;
}

// untility functions
float float_max(float a, float b)
{
	return a > b ? a : b;
}
float float_clamp(float f, float min, float max)
{
	return f < min ? min : (f > max ? max : f);
}
Vec4 Vec4_lerp(Vec4& start, Vec4& end, float alpha)
{
	return start + (end - start) * alpha;
}
Vec3 Vec3_lerp(Vec3& start, Vec3& end, float alpha)
{
	return start + (end - start) * alpha;
}
