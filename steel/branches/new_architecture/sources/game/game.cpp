/*id*********************************************************
	File: game/game.cpp
	Unit: game
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Этот юнит является дополнением к steel engine и служит
		примером использования движка. В этом юните задаются правила игры
		или графической демки.
 ************************************************************/

#include "../steel.h"

#include "game.h"
#include "../common/logger.h"
#include "../common/utils.h"

#include "../res/res_main.h"
#include "../objects/scene.h"
//#include "../objects/kar98k.h"

using namespace std;


bool Steel::init(Config *_conf, Input *_input, std::string params)
{
	conf = _conf;
	if(!conf)
	{
		log_msg("error game res", "Cannot load game config");
		return false;
	}
	if(!executeScript(params))
	{
		log_msg("core error", "Cannor execute script");
		return false;
	}

	input = _input; 
	input->setGame(this);

	// Init world
	eye = conf->getv3("camera.eye", v3(1.0f, 1.0f, 1.0f));

	v3 target = conf->getv3("camera.target", v3(0.0f,0.0f,0.0f));

	direction = target-eye;
	direction.normalize();

	accSpeed = conf->getf("camera.acc", 50);
	brakeSpeed = conf->getf("camera.brakes", 200);

	moveSpeed.loadZero();

	Config *scene = conf->find("scene");
	if(scene == NULL)
	{
		error("game res", "Cannot find scene config");
		return false;
	}

	world = createGameObject(scene);
	if(world == NULL)
	{
		error("game", "Cannot init scene");
		return false;
	}
	
	

// ******************* PHYSIC **************************
//	physicHelper  = new GraphHelper;

//	physicEngine = new PhysicEngine3D;
//	physicEngine = new PhysicEngineSteel;

/*	if(!physicEngine->init(conf->find("physic"))) return 1;
	this->bindPhysicEngine();*/

	g = conf->getv3("g");
//	physicEngine->setGravitation(g);

	_alive = true;
	paused = conf->geti("paused", 0) == 1;
	framesToPass = 0;
	speedup = 1;

	return true;
}


void Steel::handleEventKeyDown(std::string key)
{
	if(key == "escape") _alive = false;
	if(key == "pause") paused = !paused;
	if(key == "n") framesToPass = 1;

/*	if(key == "f1") physicEngine->conf->toggle("helperDrawLines");*/
	if(key == "f2") graphEngine->conf->toggle("drawFace");
	if(key == "f3") graphEngine->conf->toggle("drawWire");
	if(key == "f4") graphEngine->conf->toggle("drawBump");
	if(key == "f5") graphEngine->conf->toggle("drawVertexes");
	if(key == "f7") graphEngine->conf->toggle("drawNormals");
	if(key == "f8") graphEngine->conf->toggle("drawAABB");

	if(key == "f6") 
	{
		graphEngine->conf->toggle("clearColor");
		graphEngine->conf->toggle("clearDepth");
	}

	if(key == "1") speedup = 0.01f;
	if(key == "2") speedup = 0.05f;
	if(key == "3") speedup = 0.2f;
	if(key == "4") speedup = 0.5f;
	if(key == "5") speedup = 1;
	if(key == "6") speedup = 2;
	if(key == "7") speedup = 5;
	if(key == "8") speedup = 20;
	if(key == "9") speedup = 50;

	if(key == "g")
	{
		if(g.getSquaredLengthd()>EPSILON)
			g.loadZero();
		else
			g = conf->getv3("g");

//		physicEngine->setGravitation(g);
	}

	if(key == "mouse1" && !conf->getf("automaticGun"))
		createObject();
}

bool Steel::createObject()
{
	return false;
}

void Steel::processKeyboard()
{
	if(input->isMouseCaptured())
	{
		if(input->isPressed("mouse1") && conf->getf("automaticGun"))
			createObject();

		v3 dir(0,0,0);
		if(input->isPressed("w")) 	dir += v3(1,0,0);
		if(input->isPressed("s")) 	dir += v3(-1,0,0);
		if(input->isPressed("a")) 	dir += v3(0, +1, 0);
		if(input->isPressed("d")) 	dir += v3(0, -1, 0);
		if(input->isPressed("'")) 	dir += v3(0, 0, +1);
		if(input->isPressed("/")) 	dir += v3(0, 0, -1);

		if(dir.getLength()>EPSILON)
		{
			moveSpeed += dir*accSpeed*(float)speed;
			float len = moveSpeed.getLength();
			float maxspeed = conf->getf("camera_max_speed", 10);
			if(len > maxspeed) 
			{
				moveSpeed  = moveSpeed.getNormalized()*maxspeed;
			}
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

void Steel::handleMouse(double dx, double dy)
{
	direction.rotateZ((float)dx);

	if(dy<0 && direction.dotProduct(v3(0,0, 1))<0.99 || 
		dy>0 && direction.dotProduct(v3(0,0, -1))<0.99 )

	direction.rotateAxis((float)dy, v3( -direction.y, direction.x, 0));
}

void Steel::process(steel::time globalTime, steel::time time)
{
	processKeyboard();

	cameraSpeed = 
	moveSpeed.x*direction
	+ moveSpeed.y*v3(-direction.y, direction.x, 0).getNormalized()
	+ moveSpeed.z*v3(0, 0, 1);

	eye += cameraSpeed*(float)speed;
	
	if(!paused || framesToPass>0)
	{
		static steel::time totalPhysicTime = 0;
		steel::time frame = 0.01f*speedup;

		ProcessInfo info;
		info.curTime = totalPhysicTime;
		info.frameLength = frame;
		info.camera = &graphEngine->camera;

		world->process(info);

		totalPhysicTime += frame;

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

void Steel::bind(GraphEngine *engine)
{
	graphEngine = engine;

//	graphEngine->inject(physicHelper);

	if(world != NULL)
		engine->inject(world);
}


void Steel::draw(GraphEngine *graph)
{
	direction.normalize();

	graph->camera.setup(eye, direction);
	graph->processCamera();
	graph->process(0,0);
}



void Steel::deinit()
{
/*	if(physicEngine)
	{
		physicEngine->deinit();
		delete physicEngine;
		physicEngine = NULL;
	}
	*/
}


void Steel::handleEventKeyUp(std::string key)
{
}

bool Steel::executeScript(std::string script)
{
	log_msg("console", "ExecScript: '" + script + "'");

	svector<string> lines;
	explode(';', script, lines);
	for(svector<string>::const_iterator it = lines.begin(); it != lines.end(); it++)
		if(!executeCommand(*it)) return false;
	return true;
}

bool Steel::executeCommand(std::string command)
{
	if(command.empty()) return true;
	log_msg("console", "ExecCommand: '" + command + "'");

	// var=value

	svector<string> token;
	explode('=', command, token);
	if(token.size() != 2) return false;
	svector<string> var;
	explode('.', token[0], var);
	if(var.size() == 1)
	{
//		conf->setup(var[0], token[1]);
		log_msg("script", var[0] + " = " + token[1]);
		return true;
	}
	else
	if(var.size() == 2)
	{
// TODO: change engine params (graph, physic)
		return false;

	}
	else
		return false;
}


