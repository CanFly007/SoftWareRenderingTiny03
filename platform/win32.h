#ifndef WIN32_H
#define WIN32_H
#include <Windows.h>
#include "../core/maths.h"
typedef struct mouse
{
	// for camera orbit
	Vec2 orbit_pos;
	Vec2 orbit_delta;
	// for first-person view (diabled now)
	Vec2 fv_pos;
	Vec2 fv_delta;
	// for mouse wheel
	float wheel_delta;
}mouse_t;
typedef struct window
{
	HWND h_window;
	HDC mem_dc;
	HBITMAP bm_old;
	HBITMAP bm_dib;
	unsigned char* window_fb;
	int width;
	int height;
	char keys[512];
	char buttons[2];	//left button��0�� right button��1
	int is_close;
	mouse_t mouse_info;
}window_t;


extern window_t* window;

int window_init(int width, int height, const wchar_t* title);
int window_destroy();
void window_draw(unsigned char* framebuffer);
void msg_dispatch();
Vec2 get_mouse_pos();
float platform_get_time(void);

#endif // 
