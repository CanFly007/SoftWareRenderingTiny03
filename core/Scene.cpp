#include "./Scene.h"

//输入参数 model指针类型数组，输出参数m，输出参数shader_use
void Build_fuhua_scene(Model** model, int& m, IShader** shader_use, IShader** shader_skybox)
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
	*shader_skybox = NULL;

	for (int i = 0; i < m; i++)
	{
		model[i] = new Model(modelname[i]);
	}

	printf("scene name:%s\n", "fuhua");
	printf("model number:%d\n", m);
}
void Build_qiyana_scene(Model** model, int& m, IShader** shader_use, IShader** shader_skybox)
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
	*shader_skybox = NULL;

	for (int i = 0; i < m; i++)
	{
		model[i] = new Model(modelname[i]);
	}

	printf("scene name:%s\n", "qiyana");
	printf("model number:%d\n", m);
}
void Build_elfgirl_scene(Model** model, int& m, IShader** shader_use, IShader** shader_skybox)
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
	*shader_skybox = NULL;

	for (int i = 0; i < m; i++)
	{
		model[i] = new Model(modelname[i]);
	}

	printf("scene name:%s\n", "qiyana");
	printf("model number:%d\n", m);
}
void Build_gun_scene(Model** model, int& m, IShader** shader_use, IShader** shader_skybox)
{
	m = 2;
	const char* modelname[] =
	{
		"obj/gun/Cerberus.obj",
		"obj/skybox4/box.obj"
	};

	PhongShader* shader_phong = new PhongShader();
	*shader_use = shader_phong;
	SkyboxShader* shader_sky = new SkyboxShader();
	*shader_skybox = shader_sky;

	model[0] = new Model(modelname[0]);
	model[1] = new Model(modelname[1], true);
	//for (int i = 0; i < m; i++)
	//{
	//	model[i] = new Model(modelname[i]);
	//}

	printf("scene name:%s\n", "gun");
	printf("model number:%d\n", m);
}

void Build_PerspectiveCorrect(Model** model, int& m, IShader** shader_use, IShader** shader_skybox)
{
	m = 1;
	const char* modelname[] =
	{
		"obj/PerspectiveCorrect/CheckBoard.obj",
	};

	PhongShader* shader_phong = new PhongShader();
	*shader_use = shader_phong;
	*shader_skybox = NULL;

	model[0] = new Model(modelname[0]);
}

void Build_test_obj(Model** model, int& m, IShader** shader_use, IShader** shader_skybox)
{
	m = 1;
	const char* modelname[] =
	{
		"obj/test.obj",
	};

	PhongShader* shader_phong = new PhongShader();
	*shader_use = shader_phong;
	*shader_skybox = NULL;

	model[0] = new Model(modelname[0]);
}