#include "./Scene.h"

//输入参数 model指针类型数组，输出参数m，输出参数shader_use
void Build_fuhua_scene(Model** model, int& m, IShader** shader_use)
{
	m = 4;//这个场景有4个模型，在main中好根据m的值遍历
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
void Build_qiyana_scene(Model** model, int& m, IShader** shader_use)
{
	m = 3;
	const char* modelname[] =
	{
		"obj/qiyana/qiyanabody.obj",
		"obj/qiyana/qiyanahair.obj",
		"obj/qiyana/qiyanaface.obj",
	};

	PhongShader* shader_phong = new PhongShader();
	*shader_use = shader_phong;

	for (int i = 0; i < m; i++)
	{
		model[i] = new Model(modelname[i]);
	}

	printf("scene name:%s\n", "qiyana");
	printf("model number:%d\n", m);
}
void Build_elfgirl_scene(Model** model, int& m, IShader** shader_use)
{
	m = 7;
	const char* modelname[] =
	{
		"obj/elfgirl/base.obj",
		"obj/elfgirl/body0.obj",
		"obj/elfgirl/body1.obj",
		"obj/elfgirl/body2.obj",
		"obj/elfgirl/face0.obj",
		"obj/elfgirl/face1.obj",
		"obj/elfgirl/hair.obj",
	};
	PhongShader* shader_phong = new PhongShader();
	*shader_use = shader_phong;

	for (int i = 0; i < m; i++)
	{
		model[i] = new Model(modelname[i]);
	}

	printf("scene name:%s\n", "qiyana");
	printf("model number:%d\n", m);
}
void Build_gun_scene(Model** model, int& m, IShader** shader_use)
{
	m = 1;
	const char* modelname[] =
	{
		"obj/gun/Cerberus.obj",
	};

	PhongShader* shader_phong = new PhongShader();
	*shader_use = shader_phong;

	for (int i = 0; i < m; i++)
	{
		model[i] = new Model(modelname[i]);
	}

	printf("scene name:%s\n", "gun");
	printf("model number:%d\n", m);
}