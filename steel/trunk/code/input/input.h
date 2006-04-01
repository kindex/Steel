#ifndef INPUT_H
#define INPUT_H

/* Посредник между классом Game и драйверами, которые опрашивают
устройства ввода. Хранит события, нажатые кнопки
*/

#include "../game/game.h"

#include <string>
#include <map>


class Input
{
protected:
	std::map<std::string, bool> keyPressed;
	bool alive, mouseCaptured;
	Game *game;
public:
	bool init(std::string _conf, Game *_game) 
	{ 
		alive = true; 
		mouseCaptured = false; 
		game = _game;
		return true; 
	}

	bool isPressed(std::string key);
	bool isAlive() { return alive; }
	virtual void captureMouse(int _cx, int _cy) = 0;
	virtual void process() = 0;
	virtual void getMouseDelta(int &dx, int &dy) = 0;
};

#endif
