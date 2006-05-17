#include "model_obj.h"

bool GameObjModel::init(ScriptLine	&s, ResCollection &res)
{
	if(!GameObj::init(s, res)) return false;
	
	if(s.count()<3) return false;
	m = (Model*)res.add(Res::model, s.gets(3));
	
	if(!m) alog.msg("error res model", "Model not found: " + s.gets(3));

	float scale = getGlobalScale();
	mass = m->getVolume()*scale*scale*scale;

	return m != NULL;
}

aabb GameObjModel::getFrame()
{		
	if(m)	return m->frame;
	else	return aabb();
}

