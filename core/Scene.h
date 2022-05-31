#pragma once
#include "../shader/shader.h"

typedef struct
{
	const char* scene_name;
	//函数指针 参数1								   参数2         参数3
	//      指针数组，引用形参int(m可以在函数内改变并传出去)，传指针的地址
	void (*Build_scene)(Model** model, int& m, IShader** shader_use);
}scene_t;

void Build_fuhua_scene(Model** model, int& m, IShader** shader_use);
void Build_qiyana_scene(Model** model, int& m, IShader** shader_use);
void Build_gun_scene(Model** model, int& m, IShader** shader_use);