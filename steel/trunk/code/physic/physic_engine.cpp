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

	return true;
}

bool PhysicEngine::inject(PhysicInterface *object, matrix4 matrix)
{
	PhysicElement el;
	el.obj = object;

	objects.push_back(el);
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


bool PhysicEngine::clear()
{
	for(std::vector<PhysicElement>::iterator it = objects.begin(); it != objects.end(); it++)
		it->obj->cleanupP();

	objects.clear();
	return true;
}
