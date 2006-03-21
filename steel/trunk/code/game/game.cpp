#include "game.h"
#include "../common/logger.h"

#define MOVE_SPEED (50.0f)
#define LOOK_SPEED (0.01f)

void Game::processKeyboard()
{
	if(isPressed("w")) eye += (float)MOVE_SPEED*(float)speed*direction;
	if(isPressed("s")) eye -= (float)MOVE_SPEED*(float)speed*direction;
	if(isPressed("a"))
	{
        v3 d(direction.y, -direction.x, 0);
        d.Normalize();
        eye -= (float)MOVE_SPEED*(float)speed*d;
	}
	if(isPressed("d"))
	{
        v3 d(direction.y, -direction.x, 0);
        d.Normalize();
        eye += (float)MOVE_SPEED*(float)speed*d;
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

		for(int i=0; i<3; i++)
			obj[1]->pos.entries[i+i*4] = 0.20f;

		obj[1]->pos.entries[12] = (float)cos(time)*50.0f;
		obj[1]->pos.entries[13] = (float)sin(time)*50.0f;
		obj[1]->pos.entries[14] = 20;

		for(int i=0; i<3; i++)
			obj[1]->children[0]->pos.entries[i+i*4] = 0.20f;

		obj[1]->children[0]->pos.entries[12] = (float)cos(-time)*60.0f;
		obj[1]->children[0]->pos.entries[13] = (float)sin(-time)*60.0f;
		obj[1]->children[0]->pos.entries[14] = 20;

		graph->clear();

	    direction = 
			v3(
			cos(angle.x) * cos(angle.y),
			sin(angle.x) * cos(angle.y),
			-sin(angle.y)
			);

		graph->camera.setup(eye, direction);

		graph->processCamera();

		for(vector<GameObj*>::iterator it = obj.begin(); it != obj.end(); it++)
			graph->inject((*it));

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

	obj.resize(2);
	obj[0] = new GameObj;
	obj[0]->assignModel((Model*)res->getModel("teapot"));

	obj[1] = new GameObj;
	obj[1]->assignModel((Model*)res->getModel("teapot"));

	obj[1]->addChildren(new GameObj((Model*)res->getModel("teapot")));


//	obj[2] = new GameLight;
	
	_alive = true;
	return true;
}


void Game::handleEventKeyDown(std::string key)
{
	if(key == "escape") _alive = false;
	
	keyPressed[key] = true;
}

void Game::handleEventKeyUp(std::string key)
{
	keyPressed[key] = false;
}

bool Game::isPressed(std::string key)
{
	return keyPressed.find(key) != keyPressed.end() && keyPressed[key];
}
