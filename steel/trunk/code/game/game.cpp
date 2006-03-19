#include "game.h"
#include "../common/logger.h"

#define MOVE_SPEED (50.0f)
#define LOOK_SPEED (0.01f)

void Game::processKeyboard()
{
	if(isPressed("w")) eye += MOVE_SPEED*speed*direction;
	if(isPressed("s")) eye -= MOVE_SPEED*speed*direction;
	if(isPressed("a"))
	{
        v3 d(direction.y, -direction.x, 0);
        d.Normalize();
        eye -= MOVE_SPEED*speed*d;
	}
	if(isPressed("d"))
	{
        v3 d(direction.y, -direction.x, 0);
        d.Normalize();
        eye += MOVE_SPEED*speed*d;
	}
}

void Game::handleMouse(double dx, double dy)
{
	angle.x += (float)dx*LOOK_SPEED;
	angle.y += (float)dy*LOOK_SPEED;
	if(angle.y >  M_PI*0.45f) angle.y =  (float)M_PI*0.45f;
	if(angle.y < -M_PI*0.45f) angle.y = -(float)M_PI*0.45f;
}


void Game::process()
{
		processKeyboard();

		graph->clear();

	    direction = 
			v3(
			cos(angle.x) * cos(angle.y),
			sin(angle.x) * cos(angle.y),
			-sin(angle.y)
			);

		graph->camera.setup(eye, direction);

		graph->processCamera();

		for(vector<res_model>::iterator it = obj.begin(); it != obj.end(); it++)
			graph->inject(&(*it));

		graph->process();

}

bool Game::init()
{
	eye = v3(60, 50, 31);
	angle = v3(4.0, 0.0, 0.0);

	direction = v3(-1.0f/eye.x, -1.0f/eye.y, -1.0f/eye.z);
	direction.Normalize();

	graph->camera.seteye(eye);

	res->add(Res::model, "teapot");

	obj.resize(1);
	obj[0].assign((Model*)res->operator[]("teapot"));

	_alive = true;
	return true;
}


void Game::handleEventKeyDown(std::string key)
{
	if(key == "escape") _alive = false;
	
	keyPressed[key] = true;

//	alog.out("KeyDOWN: %s", key.c_str());
}

void Game::handleEventKeyUp(std::string key)
{
	keyPressed[key] = false;

//	alog.out("KeyUP  : %s", key.c_str());
}

bool Game::isPressed(std::string key)
{
	return keyPressed.find(key) != keyPressed.end() && keyPressed[key];
}
