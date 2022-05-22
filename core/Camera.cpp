#include "./Camera.h"

Camera::Camera(Vec3 eye, Vec3 target, Vec3 up, float aspect) :
	eye(eye), target(target), up(up), aspect(aspect) {}
Camera::~Camera() {}
