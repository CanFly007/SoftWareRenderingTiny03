#include "./Scene.h"

TGAImage* TextureFromFile(const char* fileName)
{
	TGAImage* texture = new TGAImage();
	std::cerr << "texture file " << fileName << " loading "
		<< (texture->read_tga_file(fileName) ? "ok" : "failed") << std::endl;
	texture->flip_vertically();
	return texture;
}

cubemap_t* CubemapFromFiles(const char (*paths)[256])//paths = char[6][256];
{
	cubemap_t* cubemap = new cubemap_t();
	for (int i = 0; i < 6; i++)
	{
		cubemap->faces[i] = TextureFromFile(paths[i]);
	}
	return cubemap;
}

void LoadIBLMap(payload_t& pay, const char* path)
{
	IBLMap_t* iblMap = new IBLMap_t();
	const char* faces[6] = { "px", "nx", "py", "ny", "pz", "nz" };
	char paths[6][256];

	iblMap->mipMapLevels = 10;

	for (int i = 0; i < 6; i++)
		sprintf_s(paths[i], "%s/i_%s.tga", path, faces[i]);//填充paths，比如paths[0] = "path/i_px.tga"
	iblMap->irradianceMap = CubemapFromFiles(paths);
	
	iblMap->brdfLut = TextureFromFile("obj/common/BRDF_LUT.tga");

	for (int i = 0; i < iblMap->mipMapLevels; i++)
	{
		for (int j = 0; j < 6; j++)
			sprintf_s(paths[j], "%s/m%d_%s.tga", path, i, faces[j]);

		iblMap->prefilter_maps[i] = CubemapFromFiles(paths);
	}
	pay.iblMap = iblMap;
}

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

	PBRShader* shader_PBR = new PBRShader();
	*shader_use = shader_PBR;
	SkyboxShader* shader_sky = new SkyboxShader();
	*shader_skybox = shader_sky;

	model[0] = new Model(modelname[0]);
	model[1] = new Model(modelname[1], true);
	//for (int i = 0; i < m; i++)
	//{
	//	model[i] = new Model(modelname[i]);
	//}

	LoadIBLMap(shader_PBR->payload, "obj/common2");

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