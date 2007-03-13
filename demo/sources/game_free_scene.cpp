/*id*********************************************************
	Unit: Demo
	Part of: DiVision intro
	(C) DiVision, 2004-2007
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
#include <res/config/config_setup.h>
#include <objects/combiner/combiner.h>
#include <audio/openal/eax.h>
#include <common/logger.h>
#include <common/utils.h>
#include <res/res_main.h>

bool GameFreeScene::init(Config& _conf, Input& _input)
{
	conf = &_conf;
	if (!conf)
	{
		log_msg("error game res", "Cannot load game config");
		return false;
	}

	gameObjectFactory = createGameObjectFactory();

	input = &_input; 
	input->setGame(this);

	// Init world
	spectator.camera.setPosition(conf->getv3("camera.eye", v3(0.0f, 0.0f, 1.7f)));
	spectator.camera.setDirection(conf->getv3("camera.direction", v3(1.0f, 0.0f, 0.0f)));
	
	accSpeed = conf->getf("camera.acc", 50);
	brakeSpeed = conf->getf("camera.brakes", 200);

	spectator.velocity.loadZero();

	Config* scene = conf->find("scene");
	if (scene == NULL)
	{
		error("game res", "Cannot find scene config");
		return false;
	}

	world = createGameObject(scene);
	if (world == NULL)
	{
		error("game", "Cannot init scene");
		return false;
	}
	
	Config* flashlightConf = resConfig.add("flashlight.conf");
	if (flashlightConf != NULL)
	{
		flashlight = new GameLight();
		flashlight->InitFromConfig(*flashlightConf);
		flashlight->enable();
		flashlightPosition = spectator;
		flashlight->setPosition(flashlightPosition.camera.getPosition());
	}


// ******************* PHYSIC **************************

	_alive = true;
	paused = conf->geti("paused", 0) == 1;
	framesToPass = 0;
	speedup = 1;
	speed = 0.01f;
	timeInfo.currentTime = 0.0f;
	timeInfo.frameLength = 0.0f;

	audioTimer.start(); audioTimer.pause();
	physicTimer.start(); physicTimer.pause();
	infoTimer.start(); infoTimer.pause();
	graphTimer.start(); graphTimer.pause();

	//static_cast<GameLight*>(static_cast<Combiner*>(world)->getObject(0))->toggleEnable();
//	static_cast<GameLight*>(static_cast<Combiner*>(world)->getObject(0))->toggleEnable();


	return true;
}



void GameFreeScene::handleEventKeyDown(const std::string& key)
{
	if (key == "escape") _alive = false;
	if (key == "pause") 
	{
		paused = !paused;
		if (paused)
		{
			pause();
		}
		else
		{
			unpause();
		}
	}
	if (key == "n") 
	{
		framesToPass = 1;
	}

	if (key == "f1") graphEngine->conf->toggle("shadows");
	if (key == "f2") graphEngine->conf->toggle("drawFace");
	if (key == "f3") graphEngine->conf->toggle("drawWire");
	if (key == "f4") graphEngine->conf->toggle("blend");
	if (key == "f5") graphEngine->conf->toggle("drawVertexes");
	if (key == "f7") graphEngine->conf->toggle("drawNormals");
	if (key == "f8") graphEngine->conf->toggle("drawAABB");
	if (key == "f9")
	{
		graphEngine->conf->toggle("use_debug_shader");
		executeCommand(graphEngine->conf, "debug_shader_mode = 2");
	}
	if (key == "f11")
	{
		graphEngine->conf->toggle("use_debug_shader");
		executeCommand(graphEngine->conf, "debug_shader_mode = 1");
	}
	if (key == "f12")
	{
		graphEngine->conf->toggle("use_debug_shader");
		executeCommand(graphEngine->conf, "debug_shader_mode = 0");
	}

	if (key == "f6") 
	{
		graphEngine->conf->toggle("clearColor");
		graphEngine->conf->toggle("clearDepth");
	}

	//if (key == "1") speedup = 0.01f;
	//if (key == "2") speedup = 0.05f;
	//if (key == "3") speedup = 0.2f;
	//if (key == "4") speedup = 0.5f;
	//if (key == "5") speedup = 1;
	//if (key == "6") speedup = 2;
	//if (key == "7") speedup = 5;
	//if (key == "8") speedup = 20;
	//if (key == "9") speedup = 50;

	if (key == "1")
	{
		static_cast<GameLight*>(static_cast<Combiner*>(world)->getObject(0))->toggleEnable();
	}
	if (key == "0")
	{
		static_cast<GameLight*>(static_cast<Combiner*>(world)->getObject(1))->toggleEnable();
	}
	if (key == "2")
	{
		static_cast<GameLight*>(
			static_cast<Combiner*>(static_cast<Combiner*>(world)->getObject(1))->getObject(0)
			)->toggleEnable();
	}
	if (key == "3")
	{
		static_cast<GameLight*>(
			static_cast<Combiner*>(static_cast<Combiner*>(world)->getObject(2))->getObject(0)
			)->toggleEnable();
	}

	if (key == "f")
	{
		if (flashlight != NULL)
		{
			flashlight->toggleEnable();
		}
	}
}


void GameFreeScene::processKeyboard()
{
	if (input->isMouseCaptured())
	{
		v3 dir(0,0,0);
		if (input->isPressed("w")) 	dir += v3(1,0,0);
		if (input->isPressed("s")) 	dir += v3(-1,0,0);
		if (input->isPressed("a")) 	dir += v3(0, +1, 0);
		if (input->isPressed("d")) 	dir += v3(0, -1, 0);
		if (input->isPressed("q")) 	dir += v3(0, 0, +1);
		if (input->isPressed("z")) 	dir += v3(0, 0, -1);

		if (dir.getLength() > EPSILON)
		{
			spectator.velocity += dir*accSpeed*(float)speed;
			float len = spectator.velocity.getLength();
			float maxspeed = conf->getf("camera.max_speed", 10);
			if (len > maxspeed) 
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

	if (dy<0 && dir.dotProduct(v3(0,0, 1))<0.99 || 
		dy>0 && dir.dotProduct(v3(0,0, -1))<0.99)
	{
		dir.rotateAxis((float)dy, v3(-dir.y, dir.x, 0));
	}
	spectator.camera.set(spectator.camera.getPosition(), dir, v3(0.0f, 0.0f, 1.0f));
}

void GameFreeScene::process()
{
	processKeyboard();

	v3 direction = spectator.camera.getDirection();
	v3 cameraSpeed = spectator.velocity.x*direction
					+ spectator.velocity.y*v3(-direction.y, direction.x, 0).getNormalized()
					+ spectator.velocity.z*v3(0, 0, 1);

	spectator.camera.setPosition(spectator.camera.getPosition() + cameraSpeed*(float)speed);

	updatePhysicTime();
	info.timeInfo = timeInfo;
	info.camera = spectator.camera;

	if (timeInfo.frameLength > EPSILON)
	{
		world->process(info);
		physicTimer.incframe();
	}

	if (!firstframe && infoTimer.lap() >= 0.5)
	{
		infoTimer.nextlap();
		graphEngine->setCaption(
			std::string("Sleel engine")
			+ " Batches: " + IntToStr(graphEngine->total.batchCount)
			+ " Faces: " + IntToStr(graphEngine->total.triangleCount)
			+ " Lights: " + IntToStr(graphEngine->total.lightCount)
			+ " FPS " + graphTimer.getfps_s()
		);
		speed = 1.0f/graphTimer.getfps();
		if (speed > 0.1) speed = 0.1f;
	}
	if (firstframe)
	{
		infoTimer.start();
		infoTimer.nextlap();
		graphTimer.start();
		if (!paused)
		{
			physicTimer.start();
			audioTimer.start();
		}
		firstframe = false;
	}
}

void GameFreeScene::updatePhysicTime()
{
	if (paused && framesToPass > 0)
	{
		physicTimer.add(0.01f);
	}
	
	if (!paused || framesToPass > 0)
	{
		timeInfo.currentTime = physicTimer.total();
		timeInfo.frameLength = physicTimer.lap(); physicTimer.nextlap();
		if (info.timeInfo.frameLength > 0.1f) info.timeInfo.frameLength = 0.1f;

		if (framesToPass>0)
		{
			framesToPass--;
		}
		else 
		{
			framesToPass = 0;
		}
	}
	else
	{
		timeInfo.frameLength = 0.0f;
	}
}


void GameFreeScene::bind(GraphEngine& engine)
{
	graphEngine = &engine;
	if (world != NULL)
	{
		engine.inject(world);
	}
	if (flashlight != NULL && conf->getb("flashlight"))
	{
		engine.inject(flashlight);
	}
}

void GameFreeScene::bind(AudioEngine& engine)
{
	audioEngine = &engine;
#ifdef LIB_OPENAL
	audioEngine->setListenerEnvironment(EAX_ENVIRONMENT_GENERIC); // TODO: rename const, main program must be independent from Audio Engine
#endif

	if (world != NULL)
	{
		engine.inject(*world);
	}
}


void GameFreeScene::draw(GraphEngine& graph)
{
	graphTimer.incframe();

	info.timeInfo.currentTime = graphTimer.total();
	info.timeInfo.frameLength = graphTimer.lap(); graphTimer.nextlap();


	if (!paused)
	{
		if (flashlight != NULL && conf->getb("flashlight"))
		{
			flashlightPosition.camera = spectator.camera;

			flashlight->setPosition(flashlightPosition.camera.getPosition(), 
									flashlightPosition.camera.getDirection(), 
									flashlightPosition.camera.getUpVector());
		}
	}

	graph.process(info);
}

void GameFreeScene::insonify(AudioEngine& _audioEngine)
{
	Listener listener;

	listener.setPosition(spectator.camera.getPosition());
	listener.setOrientation(spectator.camera.getDirection(), spectator.camera.getUpVector());

	_audioEngine.setListener(listener);
}


void GameFreeScene::deinit()
{
	// TODO:
}


void GameFreeScene::handleEventKeyUp(const std::string& key)
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
 	flashlight(NULL),
	speed(0.01f) // TODO: autoSpeed
{}
	
void GameFreeScene::start()
{
	firstframe = true;
}

void GameFreeScene::pause()
{
	audioTimer.pause();
	physicTimer.pause();

	if (audioEngine != NULL)
	{
		audioEngine->pause();
	}
}

void GameFreeScene::unpause()
{
	audioTimer.resume();
	physicTimer.resume();

	if (audioEngine != NULL)
	{
		audioEngine->unpause();
	}
}

