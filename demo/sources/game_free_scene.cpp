/*id*********************************************************
	Unit: Demo
	Part of: DiVision intro
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Класс для разработки и тестирования движка.
		Сцена загружается из файла. Свободно летающая камера.
 ************************************************************/

#include "game_free_scene.h"
#include <audio/openal_engine.h>

bool GameFreeScene::init(Config *_conf, Input *_input)
{
	conf = _conf;
	if(!conf)
	{
		log_msg("error game res", "Cannot load game config");
		return false;
	}

	gameObjectFactory = createGameObjectFactory();

	input = _input; 
	input->setGame(this);

	// Init world
	spectator.camera.setPosition(conf->getv3("camera.eye", v3(0.0f, 0.0f, 1.7f)));
	spectator.camera.setDirection(conf->getv3("camera.direction", v3(1.0f, 0.0f, 0.0f)));
	
	accSpeed = conf->getf("camera.acc", 50);
	brakeSpeed = conf->getf("camera.brakes", 200);

	spectator.velocity.loadZero();

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
		light->setPosition(spectator.camera.getPosition());
	}


// ******************* PHYSIC **************************

	_alive = true;
	paused = conf->geti("paused", 0) == 1;
	framesToPass = 0;
	speedup = 1;

	return true;
}



void GameFreeScene::handleEventKeyDown(std::string key)
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

	if (key == "f")
	{
		if (light != NULL)
		{
			light->toggleEnable();
		}
	}
}


void GameFreeScene::processKeyboard()
{
	if(input->isMouseCaptured())
	{
		v3 dir(0,0,0);
		if(input->isPressed("w")) 	dir += v3(1,0,0);
		if(input->isPressed("s")) 	dir += v3(-1,0,0);
		if(input->isPressed("a")) 	dir += v3(0, +1, 0);
		if(input->isPressed("d")) 	dir += v3(0, -1, 0);
		if(input->isPressed("q")) 	dir += v3(0, 0, +1);
		if(input->isPressed("z")) 	dir += v3(0, 0, -1);

		if(dir.getLength()>EPSILON)
		{
			spectator.velocity += dir*accSpeed*(float)speed;
			float len = spectator.velocity.getLength();
			float maxspeed = conf->getf("camera_max_speed", 10);
			if(len > maxspeed) 
			{
				spectator.velocity  = spectator.velocity.getNormalized()*maxspeed;
			}
		}
		else
		{
			float d = brakeSpeed*(float)speed;
			float len = spectator.velocity.getLength();
			len -= d;
			if(len < 0) len = 0;
			spectator.velocity.normalize();
			spectator.velocity *= len; 
		}
	}
}

void GameFreeScene::handleMouse(double dx, double dy)
{
	v3 dir = spectator.camera.getDirection();
	dir.rotateZ((float)dx);

	if(dy<0 && dir.dotProduct(v3(0,0, 1))<0.99 || 
		dy>0 && dir.dotProduct(v3(0,0, -1))<0.99 )

	dir.rotateAxis((float)dy, v3(-dir.y, dir.x, 0));
	spectator.camera.setDirection(dir);
}

void GameFreeScene::process(IN const TimeInfo& timeInfo)
{
	processKeyboard();

	v3 direction = spectator.camera.getDirection();
	v3 cameraSpeed = spectator.velocity.x*direction
					+ spectator.velocity.y*v3(-direction.y, direction.x, 0).getNormalized()
					+ spectator.velocity.z*v3(0, 0, 1);

	spectator.camera.setPosition(spectator.camera.getPosition() + cameraSpeed*(float)speed);
	
	if(!paused || framesToPass>0)
	{
		static steel::time totalPhysicTime = 0;
		steel::time frame = 0.01f*speedup;

		info.timeInfo = timeInfo;
		info.camera = spectator.camera;

		world->process(info);

		if(framesToPass>0)
		{
			framesToPass--;
		}
		else 
		{
			framesToPass = 0;
		}

		if(light != NULL)
		{
			light->setPosition(spectator.camera.getPosition());
		}
	}
}

void GameFreeScene::bind(GraphEngine *engine)
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

void GameFreeScene::bind(AudioEngine *engine)
{
	audioEngine = engine;
#ifdef LIB_OPENAL
	audioEngine->setListenerEnvironment(EAX_ENVIRONMENT_GENERIC);
#endif
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
	{
		engine->inject(world);
	}
}


void GameFreeScene::draw(GraphEngine *graph)
{
	graph->process(info);
}

void GameFreeScene::insonify(AudioEngine *_audioEngine)
{
	Listener listener;
	v3 eye = spectator.camera.getPosition();
	v3 direction = spectator.camera.getDirection();
	listener.setPosition(eye.x, eye.y, eye.z);
	listener.setOrientation(v3(direction.x, direction.y, direction.z), v3(0,0,1));
	_audioEngine->setListener(listener);
}


void GameFreeScene::deinit()
{
	// TODO:
}


void GameFreeScene::handleEventKeyUp(std::string key)
{
}


GameObjectFactory* GameFreeScene::createGameObjectFactory() const
{
	return new DemoGameObjectFactory;
}

GameFreeScene::GameFreeScene():
	conf(NULL),
	input(NULL),
	graphEngine(NULL),
	audioEngine(NULL),
 	world(NULL),
 	light(NULL),
	speed(0.01f) // TODO: autoSpeed
{}
	

