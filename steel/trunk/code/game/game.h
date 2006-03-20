
#ifndef GAME_H
#define GAME_H

#include "../steel.h"
#include "../graph/graph_engine.h"
#include "../graph/primiteves/res_model.h"
#include "../res/res.h"
#include <string>
#include <map>

class Game_obj: public res_model
{
public:
	MATRIX4X4 pos;

	MATRIX4X4 getMatrix()
	{
		return pos;
	}
};

class Game: public steelAbstract
{
private:
	double speed, time;
	bool _alive;
//  Input-Output 
	ResCollection *res;
	GraphEngine *graph;
	
	std::map<std::string, bool> keyPressed;
	bool isPressed(std::string key);
	void processKeyboard();

// World
	vector<Game_obj> obj;
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
	void setspeed(double _speed, double _time) {speed = _speed; time = _time; } 
};

#endif
