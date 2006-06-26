#include "physic_engine.h"
#include "../res/res_main.h"


bool PhysicEngine::init(std::string _conf)
{
	if(!(conf = resConfig.add(_conf)))
	{
		log_msg("error graph conf res","Cannot find renderer config file "+_conf);
		return false;
	}

	total.collisionCount = 0;

	return true;
}

bool PhysicEngine::inject(PhysicInterface *obj)
{
	if(!obj->beforeInject()) return false;
	objects.push_back(obj);

//	prepare(obj);

	return true;
}

bool PhysicEngine::clear()
{
	// TODO: foreach remove

	for(steel::vector<PhysicInterface*>::iterator it = objects.begin(); it != objects.end(); it++)
		(*it)->afterRemove();

	objects.clear();

	return true;
}

bool PhysicEngine::remove(PhysicInterface *_object)
{
	for(steel::vector<PhysicInterface*>::iterator it = objects.begin(); it != objects.end(); it++)
		if(*it == _object)
		{
			objects.erase(it);
			_object->afterRemove();
			return true;
		}
	return false;
}
