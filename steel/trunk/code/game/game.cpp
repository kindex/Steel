/*id*********************************************************
    Unit: game
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
		Этот юнит является дополнением к steel engine и служит
		примером использования движка. В этом юните задаются правила игры
		или графической демки.
 ************************************************************/

#include "game.h"
#include "../common/logger.h"
using namespace std;

#define MOVE_SPEED (10.0f)
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

void Game::processPhysic(PhysicEngine *physic)
{
/*		for(int i=0; i<3; i++)
			obj[1]->pos.entries[i+i*4] = 0.20f;

		obj[1]->pos.entries[12] = (float)cos(time)*50.0f;
		obj[1]->pos.entries[13] = (float)sin(time)*50.0f;
		obj[1]->pos.entries[14] = 20;
*/
		/*		for(int i=0; i<3; i++)
			obj[1]->children[0]->pos.entries[i+i*4] = 0.20f;

		obj[1]->children[0]->pos.entries[12] = (float)cos(2*time)*60.0f;
		obj[1]->children[0]->pos.entries[13] = (float)sin(2*time)*60.0f;
		obj[1]->children[0]->pos.entries[14] = 20;
*/
		for(vector<PhysicInterface*>::iterator it = pobj.begin(); it != pobj.end(); it++)
			physic->inject((*it));

		physic->process(speed);
}

void Game::process()
{
	processKeyboard();

    direction = 
		v3(
			cos(angle.x) * cos(angle.y),
			sin(angle.x) * cos(angle.y),
			-sin(angle.y)
		);

}

void Game::draw(GraphEngine *graph)
{
	graph->camera.setup(eye, direction);

	graph->processCamera();

	for(vector<GraphInterface*>::iterator it = gobj.begin(); it != gobj.end(); it++)
		graph->inject((*it));

	graph->process();
}


bool Game::init()
{
	eye = v3(10, 5, 3);
	angle = v3(4.0, 0.0, 0.0);

	direction = v3(-1.0f/eye.x, -1.0f/eye.y, -1.0f/eye.z);
	direction.Normalize();

	res->add(Res::model, "metal_teapot");
	res->add(Res::model, "room");

	gobj.push_back(new GameLight);

	GameObj *g;

	g = new GameSolidObj((Model*)res->getModel("room"));

	gobj.push_back(g);
	pobj.push_back(g);

	for(int i=0; i<10; i++)
	{
		g = new GameObj((Model*)res->getModel("box1"));
		g->setPosition(v3(float(rand()%7-3), float(rand()%7-3), float(rand()%6+1)));

		gobj.push_back(g);
		pobj.push_back(g);
	}

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
