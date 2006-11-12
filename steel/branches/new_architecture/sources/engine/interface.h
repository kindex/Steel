#ifndef ENGINE__INTERFACE_H
#define ENGINE__INTERFACE_H

#include "../steel.h"

typedef int InterfaceId;

class GameObject;

class BaseInterface
{
public:
	virtual bool setCurrentObject(GameObject*) = 0;
};

#endif
