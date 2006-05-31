#include "model_obj.h"

using namespace std;

bool GameObjModel::init(ScriptLine	&s, ResCollection &res)
{
	if(!GameObj::init(s, res)) return false;
	
	if(s.count()<3) return false;
	conf = (Config*)res.add(Res::config, s.gets(3));
	if(!conf)
	{
		alog.msg("error res model", "Model config not found: " + s.gets(3));
		return false;
	}
	string modelName = conf->gets("model");

	m = (Model*)res.add(Res::model, modelName);
	if(!m) 
	{
		alog.msg("error res model", "Model not found: " + modelName);
		return false;
	}

	float scale = getGlobalScale();
	mass = m->getVolume()*scale*scale*scale*conf->getf("density", 1.0f);

	return m != NULL;
}

aabb GameObjModel::getFrame()
{		
	if(m)	return m->frame;
	else	return aabb();
}

