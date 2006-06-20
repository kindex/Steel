#include "model_obj.h"
#include "../../common/utils.h"
using namespace std;

bool GameObjModel::init(ScriptLine	&s)
{
	if(!GameObj::init(s)) return false;
	
	if(s.count()<3) return false;
	conf = (Config*)res.add(Res::config, s.gets(3));
	if(!conf)
	{
		log_msg("error res model", "Model config not found: " + s.gets(3));
		return false;
	}
	string modelName = conf->gets("model");

	m = (Model*)res.add(Res::model, modelName);
	if(!m) 
	{
		log_msg("error res model", "Model not found: " + modelName);
		return false;
	}

	float scale = getGlobalScale();
	mass = m->getVolume()*scale*scale*scale*conf->getf("density", 1.0f);


	int i = 0;
	faceMaterial = m->faceMaterial;
	for(FaceMaterials::iterator it = faceMaterial.begin(); it != faceMaterial.end(); it++)
	{
		string material = conf->gets("material" + IntToStr(i));
		if(!material.empty())
		{
			Material* m = (Material*)res.add(Res::material, material);
			if(m)
				it->material = m;
			else
				log_msg("error model object game", "Cannot find material '" + material + "'");
		}
		i++;
		if(!it->material) 
		{
			log_msg("error game object model", "No material for object");
			return false;
		}
	}

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
