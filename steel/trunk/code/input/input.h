#ifndef INPUT_H
#define INPUT_H

/* ��������� ����� ������� Game � ����������, ������� ����������
���������� �����. ������ �������, ������� ������
*/

#include "../res/conf/conf.h"

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
public:
	bool init(ResCollection *_res, std::string _conf, Game *_game);

	bool isPressed(std::string key);
	bool isAlive() { return alive; }
	virtual void captureMouse() = 0;
	virtual void freeMouse() = 0;
	virtual void process() = 0;
	virtual void getMouseDelta(double &dx, double &dy) = 0;
	virtual void setMouseCenter(int _cx, int _cy) = 0;
};

#endif
