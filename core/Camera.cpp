#include "./Camera.h"
#include "../platform/win32.h"

Camera::Camera(Vec3 eye, Vec3 target, Vec3 up, float aspect) :
	eye(eye), target(target), up(up), aspect(aspect) {}
Camera::~Camera() {}

void handle_events(Camera& camera)
{
	if (window->keys[VK_ESCAPE])
	{
		window->is_close = 1;
	}
}
