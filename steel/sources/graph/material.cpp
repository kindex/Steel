/*id*******************************************************************************
	File: graph/material.cpp
	Unit: graph engine
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
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

MaterialStd* getMaterialClass(const std::string& _class)
{
	if (_class == "std") return new MaterialStd;
//	if (_class == "shader") return new MaterialShader;

	return NULL;
}


bool MaterialStd::InitFromConfig(Config *_conf)
{
	if(_conf == NULL) return false;
	conf = _conf;

	int blendInt = conf->geti("blend", 0);
	switch (blendInt)
	{
		case 1: blend = BLEND_ADDITIONAL_TEXTURE; break;
		case 2: blend = BLEND_WHITE; break;
		case 3: blend = BLEND_BLACK; break;
		case 0: default: blend = BLEND_NONE; break;
	}

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

	textureMatrix.texCoordsScale		= conf->getv3("texCoordsScale", v3(1.0f, 1.0f, 1.0f));
	textureMatrix.texCoordsRotation		= conf->getf ("texCoordsRotation", 0.0f);
	textureMatrix.texCoordsTranslation	= conf->getv3("texCoordsTranslation", v3(0.0f, 0.0f, 0.0f));

	diffuse_map.InitFromConfig(conf->find("diffuse_map"));
	diffuse2_map.InitFromConfig(conf->find("diffuse2_map"));
	normal_map.InitFromConfig(conf->find("normal_map"));
	specular_map.InitFromConfig(conf->find("specular_map"));
	emission_map.InitFromConfig(conf->find("emission_map"));
	env_map.InitFromConfig(conf->find("env_map"));

	color.set(conf->getv3("color", v3(1.0f, 0.0f, 0.0f))); // TODO:

	specularPower = conf->getf("specular_map_power", 16.0);
	ambient_k = conf->getf("ambient_k", 0.1f);
	dropShadows = conf->getb("dropShadows", true);
	catchShadows = conf->getb("catchShadows", true);

	return true; 
}

Config* MaterialStd::getConfig() const
{
    ConfigStruct* result = new ConfigStruct;

	result->setValue("class", new ConfigString("std"));

	result->setValue("diffuse_map", diffuse_map.getConfig());
	result->setValue("diffuse2_map", diffuse2_map.getConfig());
	result->setValue("normal_map", normal_map.getConfig());
	result->setValue("specular_map", specular_map.getConfig());
	result->setValue("emission_map", emission_map.getConfig());
	result->setValue("env_map", env_map.getConfig());

	result->setValue("texCoordsScale", createV3config(textureMatrix.texCoordsScale));
	result->setValue("texCoordsRotation", new ConfigNumber(textureMatrix.texCoordsRotation));
	result->setValue("texCoordsTranslation", createV3config(textureMatrix.texCoordsTranslation));

	result->setValue("color", createV3config(color.get()));

	result->setValue("specular_map_power", new ConfigNumber(specularPower));
	result->setValue("ambient_k", new ConfigNumber(ambient_k));
	result->setValue("dropShadows", new ConfigNumber(dropShadows));
	result->setValue("catchShadows", new ConfigNumber(catchShadows));

    return result;
}

bool TextureStd::InitFromConfig(Config* config)
{
	if (config == NULL) 
    {
        return false;
    }
	string name = config->getPath("image");
	if (!name.empty())
    {
        image = resImage.add("/" + name);
    }
	
	if (image == NULL)
	{
		return false;
	}
	k = config->getf("k", 1.0f);
	texCoordsUnit = config->geti("texCoordsUnit", 0);

	return image != NULL;
}

Config* TextureStd::getConfig() const
{
    if (image != NULL)
    {
        ConfigStruct* result = new ConfigStruct;

	    result->setValue("image", new ConfigString(image->getPath()));

        return result;
    }
    else
    {
        return NULL;
    }
}

bool MaterialStd::TextureEnv::InitFromConfig(Config *config)
{
	if (config == NULL)
    {
        return false;
    }

	string name = config->getPath("image"); 
	if (!name.empty())
    {
        cubeMap = resImage.add(name);
    }

	k = config->getf("k", 1.0f);

	string m = config->gets("type", "mirror");
	if (m == "custom")
	{
		type = TEXTURE_REFLECT_CUSTOM;
	}
	else if (m == "sky")
	{
		type = TEXTURE_REFLECT_SKY;
	}
	else if (m == "mirror")
	{
		type = TEXTURE_REFLECT_MIRROR;
	}
	else
	{
		type = TEXTURE_REFLECT_NONE;
	}

	return cubeMap != NULL;
}

Config* MaterialStd::TextureEnv::getConfig() const
{
	if (type != TEXTURE_REFLECT_NONE)
	{
		ConfigStruct* result = new ConfigStruct;

        if (cubeMap != NULL)
        {
		    result->setValue("image", new ConfigString(cubeMap->getPath()));
        }
		std::string m;
		switch (type)
		{
			case TEXTURE_REFLECT_CUSTOM: m = "custom"; break;
			case TEXTURE_REFLECT_SKY:    m = "sky";    break;
			case TEXTURE_REFLECT_MIRROR: m = "mirror"; break;
		}
		result->setValue("type", new ConfigString(m));
		result->setValue("k", new ConfigNumber(k));

		return result;
	}
	else
	{
		return NULL;
	}
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


MaterialStd* createMaterial(Config* conf)
{
	if (conf == NULL)
    {
        return NULL;
    }

	MaterialStd* material = getMaterialClass(conf->gets("class"));
	if (material == NULL)
    {
        return NULL;
    }

	material->InitFromConfig(conf);

	return material;
}

MaterialStd* createMaterial(const std::string& path)
{
	return createMaterial(resConfig.add(path));
}

