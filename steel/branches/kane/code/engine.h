
#ifndef ENGINE_H
#define ENGINE_H

#include <vector>
using namespace std;

class Interface;

typedef vector<Interface*> InterfaceList;


class Interface
{
public:
};


class Engine
{
public:
	virtual ~Engine() {}
	virtual bool init() = 0; // initialization
//	virtual bool inject(Interface *object) { return false; }; // add object to process by engine
	virtual bool clear() = 0; 
	virtual bool process() = 0; // add object to process by engine
	virtual bool deinit() = 0;
};

#endif
