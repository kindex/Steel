
#ifndef GAME_H
#define GAME_H

#include "../steel.h"
#include "../graph/graph_engine.h"
#include "../graph/primiteves/res_model.h"
#include "../res/res.h"
#include <string>
#include <map>

class Game: public steelAbstract
{
private:
	double speed;
	bool _alive;
//  Input-Output 
	ResCollection *res;
	GraphEngine *graph;
	
	std::map<std::string, bool> keyPressed;
	bool isPressed(std::string key);
	void processKeyboard();

// World
	vector<res_model> obj;
// Camera
	v3	eye, direction, angle;

public:
	Game(ResCollection *ares, GraphEngine *agraph) { res = ares; graph = agraph; } 
	bool init();
	void process();
	void handleEventKeyDown(std::string key);
	void handleEventKeyUp(std::string key);
	void handleMouse(double dx, double dy);
	bool alive() {return _alive;} 
	void setspeed(double _speed) {speed = _speed;} 
};

#endif
