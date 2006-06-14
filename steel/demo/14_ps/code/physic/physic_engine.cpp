#include "physic_engine.h"


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
	if(!obj->beforeInject()) return false;
	object.push_back(obj);

//	prepare(obj);

	return true;
}

bool PhysicEngine::clear()
{
	// TODO: foreach remove

	for(steel::vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		(*it)->afterRemove();

	object.clear();

	return true;
}

bool PhysicEngine::remove(PhysicInterface *_object)
{
	for(steel::vector<PhysicInterface*>::iterator it = object.begin(); it != object.end(); it++)
		if(*it == _object)
		{
			object.erase(it);
			_object->afterRemove();
			return true;
		}
	return false;
}
