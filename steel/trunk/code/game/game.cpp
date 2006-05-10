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

void Game::handleEventKeyDown(std::string key)
{
	if(key == "escape") _alive = false;
	if(key == "pause") paused = !paused;
	if(key == "n") framesToPass = 1;
	if(key == "f2") graphEngine->conf->toggle("drawTexture");
	if(key == "f3") graphEngine->conf->toggle("drawWire");
	if(key == "f4") graphEngine->conf->toggle("drawAABB");
	if(key == "f5") graphEngine->conf->toggle("drawVertexes");

	if(key == "1") speedup = 0.01;
	if(key == "2") speedup = 0.05;
	if(key == "3") speedup = 0.2;
	if(key == "4") speedup = 0.5;
	if(key == "5") speedup = 1;
	if(key == "6") speedup = 2;
	if(key == "7") speedup = 5;
	if(key == "8") speedup = 20;
	if(key == "9") speedup = 50;
}


void Game::processKeyboard()
{
	if(input->isMouseCaptured())
	{
		v3 dir(0,0,0);
		if(input->isPressed("w")) 
			dir += v3(1,0,0);
		if(input->isPressed("s")) 
			dir += v3(-1,0,0);
		if(input->isPressed("a")) 
			dir += v3(0, +1, 0);
		if(input->isPressed("d")) 
			dir += v3(0, -1, 0);
		if(input->isPressed("'")) 
			dir += v3(0, 0, +1);
		if(input->isPressed("/")) 
			dir += v3(0, 0, -1);

		if(dir.getLength()>EPSILON)
		{ 
			moveSpeed += dir*accSpeed*(float)speed;
		}
		else
		{
			float d = brakeSpeed*(float)speed;
			float len = moveSpeed.getLength();
			len -= d;
			if(len < 0) len = 0;
			moveSpeed.normalize();
			moveSpeed *= len; 
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

	eye += moveSpeed.x*direction*(float)speed;
	eye += moveSpeed.y*v3(-direction.y, direction.x, 0).getNormalized()*(float)speed;
	eye += moveSpeed.z*v3(0, 0, 1)*(float)speed;
/*
	v3 d(direction.y, -direction.x, 0);
	d.normalize();
	eye += (float)(moveSpeed.w-moveSpeed.z)*(float)speed*d;
*/

	if(!paused || framesToPass>0)
	{
		static steel::time totalPhysicTime = 0;
		steel::time frame = 0.01*speedup;
 		physic->process(totalPhysicTime, frame);
		totalPhysicTime += frame;

//		physic->process(globalTime, time);

		if(framesToPass>0) framesToPass--;
		else framesToPass = 0;
	}
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
	graphEngine = graph;
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

	accSpeed = conf->getf("camera.acc", 100);
	brakeSpeed = conf->getf("camera.brakes", 200);

	moveSpeed.loadZero();

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
	paused = conf->geti("paused", 0) == 1;
	framesToPass = 0;
	speedup = 1;

	return true;
}

void Game::handleEventKeyUp(std::string key)
{
}

