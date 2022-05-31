#pragma once
#include "../shader/shader.h"

typedef struct
{
	const char* scene_name;
	//����ָ�� ����1								   ����2         ����3
	//      ָ�����飬�����β�int(m�����ں����ڸı䲢����ȥ)����ָ��ĵ�ַ
	void (*Build_scene)(Model** model, int& m, IShader** shader_use);
}scene_t;

void Build_fuhua_scene(Model** model, int& m, IShader** shader_use);
void Build_qiyana_scene(Model** model, int& m, IShader** shader_use);
void Build_gun_scene(Model** model, int& m, IShader** shader_use);