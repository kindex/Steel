
#ifndef ENGINE_H
#define ENGINE_H

#include <vector>

class Interface;

typedef std::vector<Interface*> InterfaceList;


class Interface
{
public:
};


class Engine
{
public:

	virtual bool init() = 0; // initialization
//	virtual bool inject(Interface *object) { return false; }; // add object to process by engine
	virtual bool clear() = 0; 
	virtual bool process() = 0; // add object to process by engine
	virtual bool deinit() = 0;
};

#endif
