/*id*********************************************************
	File: objects/console.h
	Unit: objects
	Part of: Steel Engine
	(C) DiVision, 2008
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Консоль для вывода отладочной информации на экран в графическом режиме
 ************************************************************/
#ifndef OBJECTS__CONSOLE_H
#define OBJECTS__CONSOLE_H

#include "../steel.h"
#include "../graph/graph_interface.h"
#include "../engine/game_object.h"
#include <deque>

class Console: public GameObject
{
public:
	bool supportsInterface(Engine& engine, IN const InterfaceId id)
	{
		return id == INTERFACE_GRAPH;
	}
	bool updateInformation(IN OUT Engine&, IN const InterfaceId);
	void process(IN const ProcessInfo&){}
    bool InitFromConfig(IN Config&) { return false; }
    Config* getConfig() const { return NULL; }
    void add(const std::string& line);

private:
    std::deque<std::string> lines;
};

extern Console console;

#endif
