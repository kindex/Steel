#ifndef __GAME_CUSTOM_PATH_H
#define __GAME_CUSTOM_PATH_H

#include "game_obj.h"

/*
Path. �����, �������� ������� ������������ ��������.
*/
class CustomPath: public GameObj
{
public:
	
	void	process(steel::time curTime, steel::time frameLength, PhysicEngine *engine);
};

#endif
