#pragma once
#include "../shader/shader.h"

typedef struct
{
	const char* scene_name;
	//����ָ�� ����1								   ����2         ����3
	//      ָ�����飬�����β�int(m�����ں����ڸı䲢����ȥ)����ָ��ĵ�ַ
	void (*Build_scene)(Model** model, int& m, IShader** shader_use, IShader** shader_skybox);
}scene_t;

void LoadIBLMap(payload_t& pay, const char* path);

void Build_fuhua_scene(Model** model, int& m, IShader** shader_use, IShader** shader_skybox);
void Build_qiyana_scene(Model** model, int& m, IShader** shader_use, IShader** shader_skybox);
void Build_elfgirl_scene(Model** model, int& m, IShader** shader_use, IShader** shader_skybox);
void Build_gun_scene(Model** model, int& m, IShader** shader_use, IShader** shader_skybox);

void Build_PerspectiveCorrect(Model** model, int& m, IShader** shader_use, IShader** shader_skybox);
void Build_test_obj(Model** model, int& m, IShader** shader_use, IShader** shader_skybox);
