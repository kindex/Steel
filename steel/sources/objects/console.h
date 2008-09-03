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
#include "../common/timer.h"

#include <deque>

class Console: public GameObject
{
public:
    Console();
	bool supportsInterface(Engine& engine, IN const InterfaceId id)
	{
		return id == INTERFACE_GRAPH;
	}
	bool updateInformation(IN OUT Engine&, IN const InterfaceId);
    void process(IN const ProcessInfo&) {}
    void process();
    bool InitFromConfig(IN Config&) { return false; }
    Config* getConfig() const { return NULL; }
    void write(const std::string& message, const color4f color = color4f(1, 1, 1, 1));
    void clear();
    void shift();

private:
    struct Line
    {
        Line(const std::string& message, const color4f color, const steel::time timestamp);
        std::string message;
        color4f     color;
        steel::time timestamp;
    };
    std::deque<Line> lines;
    size_t           line_count;
    Timer            timer;
};

extern Console console;

#endif
