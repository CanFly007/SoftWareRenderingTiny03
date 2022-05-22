#ifndef CAMERA_H
#define CAMERA_H
#include "./Maths.h"

class Camera
{
public:
	Camera(Vec3 eye, Vec3 target, Vec3 up, float aspect);
	~Camera();

	Vec3 eye;
	Vec3 target;
	Vec3 up;
	float aspect;
};
#endif // !CAMERA_H
