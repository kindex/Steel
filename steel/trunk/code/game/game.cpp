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
#include "../common/utils.h"

#include "../res/script/script.h"

using namespace std;

//#define MOVE_SPEED (10.0f)
//#define LOOK_SPEED (0.01f)

void Game::processKeyboard()
{
	if(input->isMouseCaptured())
	{
		if(input->isPressed("w")) eye += (float)moveSpeed*(float)speed*direction;
		if(input->isPressed("s")) eye -= (float)moveSpeed*(float)speed*direction;
		if(input->isPressed("a"))
		{
			v3 d(direction.y, -direction.x, 0);
			d.normalize();
			eye -= (float)moveSpeed*(float)speed*d;
		}
		if(input->isPressed("d"))
		{
			v3 d(direction.y, -direction.x, 0);
			d.normalize();
			eye += (float)moveSpeed*(float)speed*d;
		}
	}
}

void Game::handleMouse(double dx, double dy)
{
	direction.rotateZ((float)dx);

	if(dy<0 && direction.dotProduct(v3(0,0, 1))<0.9 || 
		dy>0 && direction.dotProduct(v3(0,0, -1))<0.9 )

		direction.rotateAxis((float)dy, v3( -direction.y, direction.x, 0));
}

void Game::process(PhysicEngine *physic, steel::time globalTime, steel::time time)
{
	processKeyboard();
	physic->process(globalTime, time);
}

/*v3	Game::getGlobalPosition(std::string obj)
{
	matrix4 global;
	GameObj *g = tag[obj];
	while(g)
	{
		matrix4 local = g->getPMatrix();
		global = local*global;
		g = g->getParent();
	}
	return global*v3();
}
*/


void Game::bind(GraphEngine *graph)
{
	graph->inject(world);
}

void Game::bind(PhysicEngine *physic)
{
	physic->inject(world);
}

void Game::draw(GraphEngine *graph)
{
/*	if(!input->isMouseCaptured())
	{
		if(tag.find("camera.eye") != tag.end())
			eye = getGlobalPosition("camera.eye");
		if(tag.find("camera.target") != tag.end())
		{
			v4 target = getGlobalPosition("camera.target");
			direction = target - eye;
		}
	}
*/
	direction.normalize();

	graph->camera.setup(eye, direction);
	graph->processCamera();

	graph->process();
}


bool Game::init(ResCollection *_res, string _conf, Input *_input)
{
	res = _res;	
	input = _input; 
	input->setGame(this);
	conf = (Config*)res->add(Res::config, _conf);
	if(!conf)
	{
		alog.msg("error game res", string("Cannot load game config ") + _conf);
		return false;
	}

	eye = v3(conf->getf("camera.eye.x"), conf->getf("camera.eye.y"), conf->getf("camera.eye.z"));

	v3 target = v3(conf->getf("camera.target.x"), conf->getf("camera.target.y"), conf->getf("camera.target.z"));

	direction = target-eye;
	direction.normalize();

	moveSpeed = conf->getd("camera.speed", 1);

	if(!conf->isset("script"))
	{
		alog.msg("error game res", "Cannot find script to init scene");
		return false;
	}

	world = new GameGroup();
	world->setProcessKind(ProcessKind::none);
	
//	Interface::global, 

	if(!world->load(conf->gets("script"), res)) return false;

	_alive = true;
	return true;
}


void Game::handleEventKeyDown(std::string key)
{
	if(key == "escape") _alive = false;
}

void Game::handleEventKeyUp(std::string key)
{
}

