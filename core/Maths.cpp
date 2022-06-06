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
Vec3 Mat3::operator[](int i)const { return rows[i]; }//右值，const的
Vec3& Mat3::operator[](int i) { return rows[i]; }//左值，可赋值的
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
//行列式的值，等于任意一行（一列）的各元素乘以它对应的代数余子式乘积之和
static float Mat3_determinant(const Mat3& m)
{
	float a = +m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]);
	float b = -m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]);
	float c = +m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
	return a + b + c;
}
//伴随矩阵：代数余子式按列排序（即按行排序后再转置一次）
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
	adjoint = adjoint.transpose();//按列排序才是伴随矩阵，所以转置一下	return adjoint;
	return adjoint;
}
//逆矩阵：伴随矩阵除以矩阵行列式的值
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
//返回r行c列上代数余子式的值
//代数余子式：去除第r行和第c列，余下的3x3所构成的行列式称为余子式，前面乘以正负号即是代数余子式
static float Mat4_algebraicComplement(const Mat4& m4, int r, int c)
{
	Mat3 complement;//余子式形式的矩阵
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int row = i < r ? i : i + 1;
			int col = j < c ? j : j + 1;
			complement[i][j] = m4[row][col];
		}
	}
	float determinant = Mat3_determinant(complement);//余子式的值
	int sign = (r + c) % 2 == 0 ? 1 : -1;
	return sign * determinant;
}
//行列式的值：任意一行（或一列）各个元素乘以它对应的代数余子式乘积之和
static float Mat4_determinant(const Mat4& m)
{
	float determinant = 0;
	for (int j = 0; j < 4; j++)//按第0行各个元素乘积之和
		determinant += m[0][j] * Mat4_algebraicComplement(m, 0, j);
	return determinant;
}
//伴随矩阵：代数余子式按列排序（即按行排序后再转置一次）
static Mat4 Mat4_adjoint(const Mat4& m)
{
	Mat4 adjoint;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			adjoint[i][j] = Mat4_algebraicComplement(m, i, j);
	adjoint = adjoint.transpose();
	return adjoint;
}
//逆矩阵：伴随矩阵除以矩阵行列式的值
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
		viewMat[0][j] = x[j];//按行排列
		viewMat[1][j] = y[j];
		viewMat[2][j] = z[j];
	}
	//先反向平移(-cameraPos放到第四列)，再旋转，所以第四列是：上面矩阵每一行和第四列点积
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
	ortho[2][2] = 2.0 / (near - far);//负数，右手转左手
	ortho[2][3] = (near + far) / (near - far);//z方向平移到原点
	return ortho;
}
 //*           2*near/width              0             0           0
 //*                      0  2*near/height             0           0
 //*                      0              0    (n+f)/(n-f)   2fn/(n-f)
 //*                      0              0            -1           0
Mat4 PerspectiveProjection(float FOV, float aspect, float near, float far)
{
	//根据FOV和宽高比，算出宽和长
	float halfFOV = FOV / 2;//FOV是整个的角度
	halfFOV = halfFOV / 180.0 * PI;//转换成弧度
	float halfHeight = tan(halfFOV) * near;
	float height = halfHeight * 2.0;
	float width = aspect * height;

	Mat4 perspective = Mat4::identity();
	//ViewSpace下w分量是1，ClipSpace下w分量是-Z，所以透视变换矩阵的第四行第三列是-1
	//ViewSpace下x和y分量先通过相似三角形投影到摄像机的近平面，然后从[-width,width]压扁到[-1,1]，注意要提出除数-Z（作为透视除法ClipSpace->NDC)
	//ViewSpace下z分量：设三行三列为A,三行四列为B。分别将(-n,-1)和(-f,1)代入Zn = (A * Ze + B)/(-Ze)中，解出A和B的值
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
