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

	g.z = -(float)conf->getf("g", 0.0);
	g.x = 0.0f;
	g.y = 0.0f;
	
	return true;
}

bool PhysicEngine::inject(PhysicInterface *object)
{
	objects.push_back(object);
/*	MATRIX4X4 cur_matrix, new_matrix;

	cur_matrix = object->getMatrix();
	new_matrix = matrix*cur_matrix;
*/
	PhysicInterface &o = *(PhysicInterface*)object;
	PhysicInterfaceList children = o.getPChildrens();
	for(PhysicInterfaceList::iterator it=children.begin();	it != children.end(); it++)
		if(!inject(*it)) return false;

/*	for(FaceMaterials::iterator it = m->begin(); it != m->end(); it++)
	{
2D hashing
	}
*/
	return true;
}

bool PhysicEngine::process(steel::time speed)
{
	v3 acc = g*(float)speed;
	for(std::vector<PhysicInterface*>::iterator it = objects.begin(); it != objects.end(); it++)
	{
		v3 v = (*it)->getVelocity();
		v += acc;
		(*it)->setVelocity(v);

/*		matrix4 m = (*it)->getMatrix();
		p += v*speed;
		(*it)->setPosition(p);*/
	}

	return true;
}

bool PhysicEngine::clear()
{
	for(std::vector<PhysicInterface*>::iterator it = objects.begin(); it != objects.end(); it++)
		(*it)->cleanupP();

	objects.clear();
	return true;
}
