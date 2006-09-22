/*id*********************************************************
	File: objects/model_obj.cpp
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Статическая модель, котоаря рисуется и движется
 ************************************************************/

#include "../steel.h"
#include "model_obj.h"
#include "../common/utils.h"
#include "../res/res_main.h"

using namespace std;

bool GameObjModel::init(ScriptLine	&s)
{
	if(!GameObj::init(s)) return false;
	
	if(s.count()<3) return false;
	string config = s.gets(3);
	conf = resOldConfig.add(config, false);
	if(!conf)
	{
		log_msg("error res model", "Model config not found: " + s.gets(3));
		resOldConfig.pop();
		return false;
	}
	string modelName = conf->gets("model");

	m = resModel.add(modelName);
	if(!m) 
	{
		log_msg("error res model", "Model not found: " + modelName);
		resOldConfig.pop();
		return false;
	}

	float scale = s.getf(6, 1.0f);
	mass = m->getVolume()*scale*scale*scale*conf->getf("density", 1.0f);


	int i = 0;
	faceMaterial = m->faceMaterial;
	for(FaceMaterials::iterator it = faceMaterial.begin(); it != faceMaterial.end(); it++)
	{
		string material = conf->gets("material" + IntToStr(i));
		if(!material.empty())
		{
			Material* m = resMaterial.add(material);
			if(m)
				it->material = m;
			else
				log_msg("error model object game", "Cannot find material '" + material + "'");
		}
		i++;
		if(!it->material) 
		{
			log_msg("error game object model", "No material for object");
			resOldConfig.pop();
			return false;
		}
	}

	resOldConfig.pop();
	return m != NULL;
}

aabb GameObjModel::getFrame()
{		
	if(m)	return m->frame;
	else	return aabb();
}


FaceMaterials* GameObjModel::getFaceMaterials()
{
	if(m)
		return &faceMaterial;
	else
		return NULL;
}
