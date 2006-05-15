#include "physic_engine.h"

using namespace std;

bool PhysicEngine::init(std::string _conf)
{
	if(!res->add(Res::config, _conf))
	{
		alog.msg("error graph conf res","Cannot find renderer config file "+_conf);
		return false;
	}

	conf = (Config*)res->get(Res::config, _conf);

	total.collisionCount = 0;

	return true;
}

bool PhysicEngine::inject(PhysicInterface *obj)
{
	object.push_back(obj);

//	prepare(obj);

	return true;
}

bool PhysicEngine::clear()
{
/*	for(std::vector<Element>::iterator it = objects.begin(); it != objects.end(); it++)
		it->obj->cleanupP();
*/
	object.clear();
	element.clear();

	return true;
}
