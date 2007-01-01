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
#include "../audio/openal_engine.h"
#include "../res/config/config_parser.h"


using namespace std;


bool Steel::init(Config *_conf, Input *_input)
{
	conf = _conf;
	if(!conf)
	{
		log_msg("error game res", "Cannot load game config");
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
	
	light = new GameLight();
	light->InitFromConfig(resConfig.add("flashlight.conf"));
	light->enable();
	if(light != NULL)
	{
		light->setPosition(eye);
	}

// ******************* PHYSIC **************************

	_alive = true;
	paused = conf->geti("paused", 0) == 1;
	framesToPass = 0;
	speedup = 1;

	return true;
}

Steel::Steel(void):
	input(NULL),
	conf(NULL),
	graphEngine(NULL),
	world(NULL),
	light(NULL)
{} 


void Steel::handleEventKeyDown(std::string key)
{
	if (key == "escape") _alive = false;
	if (key == "pause") paused = !paused;
	if (key == "n") framesToPass = 1;

/*	if(key == "f1") physicEngine->conf->toggle("helperDrawLines");*/
	if (key == "f2") graphEngine->conf->toggle("drawFace");
	if (key == "f3") graphEngine->conf->toggle("drawWire");
	if (key == "f4") graphEngine->conf->toggle("drawBump");
	if (key == "f5") graphEngine->conf->toggle("drawVertexes");
	if (key == "f7") graphEngine->conf->toggle("drawNormals");
	if (key == "f8") graphEngine->conf->toggle("drawAABB");

	if (key == "f6") 
	{
		graphEngine->conf->toggle("clearColor");
		graphEngine->conf->toggle("clearDepth");
	}

	if (key == "1") speedup = 0.01f;
	if (key == "2") speedup = 0.05f;
	if (key == "3") speedup = 0.2f;
	if (key == "4") speedup = 0.5f;
	if (key == "5") speedup = 1;
	if (key == "6") speedup = 2;
	if (key == "7") speedup = 5;
	if (key == "8") speedup = 20;
	if (key == "9") speedup = 50;

	if (key == "g")
	{
		if (g.getSquaredLengthd()>EPSILON)
			g.loadZero();
		else
			g = conf->getv3("g");

//		physicEngine->setGravitation(g);
	}

	if (key == "f")
	{
		if (light != NULL)
		{
			light->toggleEnable();
		}
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
		if(input->isPressed("q")) 	dir += v3(0, 0, +1);
		if(input->isPressed("z")) 	dir += v3(0, 0, -1);

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

//		totalPhysicTime += frame;
		totalPhysicTime = globalTime;

		if(framesToPass>0) framesToPass--;
		else framesToPass = 0;

		if(light != NULL)
		{
			light->setPosition(eye);
		}
	}
}

void Steel::bind(GraphEngine *engine)
{
	graphEngine = engine;
	if(world != NULL)
	{
		engine->inject(world);
	}
	if(light != NULL)
	{
		engine->inject(light);
	}
}

void Steel::bind(AudioEngine *engine)
{
	audioEngine = engine;
//#ifdef LIB_OPENAL
	audioEngine->setListenerEnvironment(EAX_ENVIRONMENT_GENERIC);
//#endif
	/*
	EAX_ENVIRONMENT_GENERIC,
    EAX_ENVIRONMENT_PADDEDCELL,
    EAX_ENVIRONMENT_ROOM,
    EAX_ENVIRONMENT_BATHROOM,
    EAX_ENVIRONMENT_LIVINGROOM,
    EAX_ENVIRONMENT_STONEROOM,
    EAX_ENVIRONMENT_AUDITORIUM,
    EAX_ENVIRONMENT_CONCERTHALL,
    EAX_ENVIRONMENT_CAVE,
    EAX_ENVIRONMENT_ARENA,
    EAX_ENVIRONMENT_HANGAR,
    EAX_ENVIRONMENT_CARPETEDHALLWAY,
    EAX_ENVIRONMENT_HALLWAY,
    EAX_ENVIRONMENT_STONECORRIDOR,
    EAX_ENVIRONMENT_ALLEY,
    EAX_ENVIRONMENT_FOREST,
    EAX_ENVIRONMENT_CITY,
    EAX_ENVIRONMENT_MOUNTAINS,
    EAX_ENVIRONMENT_QUARRY,
    EAX_ENVIRONMENT_PLAIN,
    EAX_ENVIRONMENT_PARKINGLOT,
    EAX_ENVIRONMENT_SEWERPIPE,
    EAX_ENVIRONMENT_UNDERWATER,
    EAX_ENVIRONMENT_DRUGGED,
    EAX_ENVIRONMENT_DIZZY,
    EAX_ENVIRONMENT_PSYCHOTIC
	*/
	if(world != NULL)
		engine->inject(world);
}


void Steel::draw(GraphEngine *graph)
{
	direction.normalize();

	graph->camera.setPosition(eye);
	graph->camera.setDirection(direction);
	graph->processCamera();
	graph->process(0,0);
}

void Steel::insonify(AudioEngine *_audioEngine)
{
	Listener listener;
	listener.setPosition(eye.x, eye.y, eye.z);
	listener.setOrientation(v3(direction.x, direction.y, direction.z), v3(0,0,1));
	_audioEngine->setListener(listener);
}


void Steel::deinit()
{
}


void Steel::handleEventKeyUp(std::string key)
{
}
