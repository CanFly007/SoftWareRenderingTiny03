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
	Vec3 x;
	Vec3 y;
	Vec3 z;
};

//handle event
void updata_camera_pos(Camera& camera);
void handle_events(Camera& camera);
#endif // !CAMERA_H
