#include "./Scene.h"

//������� modelָ���������飬�������m���������shader_use
void Build_fuhua_scene(Model** model, int& m, IShader** shader_use)
{
	m = 4;//���������4��ģ�ͣ���main�кø���m��ֵ����
	const char* modelname[] =
	{
		"obj/fuhua/fuhuabody.obj",
		"obj/fuhua/fuhuahair.obj",
		"obj/fuhua/fuhuaface.obj",
		"obj/fuhua/fuhuacloak.obj",
	};

	PhongShader* shader_phong = new PhongShader();
	*shader_use = shader_phong;

	for (int i = 0; i < m; i++)
	{
		model[i] = new Model(modelname[i]);
	}

	printf("scene name:%s\n", "fuhua");
	printf("model number:%d\n", m);
}