/*id*******************************************************************************
	File: graph/material.cpp
	Unit: graph engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
        Формат хранения материала
**************************************************************************************/

#include "../steel.h"
#include "material.h"
#include "../res/res_main.h"
#include "../common/utils.h"
using namespace std;

Material *getMaterialClass(std::string _class)
{
	if (_class == "std") return new MaterialStd;
//	if (_class == "shader") return new MaterialShader;

	return NULL;
}


bool MaterialStd::InitFromConfig(Config *_conf)
{
	if(_conf == NULL) return false;
	conf = _conf;

	blend = conf->geti("blend") > 0;

/*	TextureBlendMode mode;
	string sMode = conf->gets("mode");
	mode = TEXTURE_BLEND_MODE_NONE;
	if(sMode == "")	mode = TEXTURE_BLEND_MODE_REPLACE;
	if(sMode == "+")mode = TEXTURE_BLEND_MODE_ADD;
	if(sMode == "*")mode = TEXTURE_BLEND_MODE_MUL;
	if(sMode == "blend")mode = TEXTURE_BLEND_MODE_BLEND;

	texture->format = TEXTURE_FORMAT_COLOR_MAP;
*/
	string name;

	diffuse_map.InitFromConfig(conf->find("diffuse_map"));
	diffuse2_map.InitFromConfig(conf->find("diffuse2_map"));
	normal_map.InitFromConfig(conf->find("normal_map"));
//	reflect_map.InitFromConfig(conf->find("reflect_map"));
	specular_map.InitFromConfig(conf->find("specular_map"));
	emission_map.InitFromConfig(conf->find("emission_map"));

	color.set(conf->getv3("color", v3(1.0f, 0.0f, 0.0f))); // TODO:

	specularPower = conf->getf("specular_map.power", 16.0);

	return true; 
}

bool MaterialStd::TextureStd::InitFromConfig(Config *config)
{
	if(config == NULL) return false;
	string name = "/" + config->getPath("image"); 
	if(!name.empty()) image = resImage.add(name);
	
	if(image == NULL)
	{
		return false;
	}
	k					= config->getf ("k", 1.0f);

	texCoordsUnit		= config->geti ("texCoordsUnit", 0);
	textureMatrix.texCoordsScale		= config->getv3("texCoordsScale", v3(1.0f, 1.0f, 1.0f));
	textureMatrix.texCoordsRotation	= config->getf ("texCoordsRotation", 0.0f);
	textureMatrix.texCoordsTranslation= config->getv3("texCoordsTranslation", v3(0.0f, 0.0f, 0.0f));

	return image != NULL;
}

bool MaterialStd::TextureReflect::InitFromConfig(Config *config)
{
	if(config == NULL) return false;
	string name = "/" + config->getPath("image"); 
	if(!name.empty()) image = resImage.add(name);


	string m = config->gets("type", "mirror");
	if(m == "sky") type = TEXTURE_REFLECT_SKY;
	if(m == "mirror") type = TEXTURE_REFLECT_SKY;
	else type = TEXTURE_REFLECT_MIRROR;

	return image != NULL;

}


MaterialStd::MaterialStd(): 
	Material(MATERIAL_STD)
{}

Material::~Material()
{
	 // TODO: cleanup
/*	for(svector<Texture*>::iterator it = textures.begin(); it != textures.end(); it++)
	{
		if(it->image) resImage.remove(it->image);
	}
	if(conf) resConfig.remove(conf);*/
}


Material *createMaterial(Config* conf)
{
	if(conf == NULL) return NULL;

	Material *material = getMaterialClass(conf->gets("class"));
	if(material == NULL) return NULL;

	material->InitFromConfig(conf);
	return material;
}

Material *createMaterial(std::string path)
{
	return createMaterial(resConfig.add(path));
}
