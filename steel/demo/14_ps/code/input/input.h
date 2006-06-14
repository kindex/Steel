#ifndef INPUT_H
#define INPUT_H

/* Посредник между классом Game и драйверами, которые опрашивают
устройства ввода. Хранит события, нажатые кнопки
*/

#include "../res/conf/conf.h"

class Input;  // forward declaration, cross-use
#include "../game/game.h"

#include <string>
#include <map>


class Input
{
protected:
	std::map<std::string, bool> keyPressed;
	double sensetivity;
	bool alive, mouseCaptured, focused;
	Game	*game;
	Config	*conf;
	ResCollection		*res;

	int cx, cy, lastdx, lastdy;

public:
	bool init(ResCollection *_res, std::string _conf);
	void setGame(Game *_game) { game = _game; }

	bool isPressed(std::string key);
	bool isAlive() { return alive; }
	virtual void captureMouse() = 0;
	virtual void freeMouse() = 0;
	virtual void process() = 0;
	virtual void getMouseDelta(double &dx, double &dy);
	virtual void setMouseCenter(int _cx, int _cy) { cx = _cx; cy = _cy; }
	bool isMouseCaptured() { return mouseCaptured; }
};

#endif
