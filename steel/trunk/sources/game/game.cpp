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

#include "../res/script/script.h"

#include "../objects/model_obj.h"
#include "../objects/combiner.h"
#include "../objects/sphere.h"
#include "../objects/audio_object.h"
#include "../objects/audio_ambient.h"

#include "../res/res_main.h"

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
	eye = v3(conf->getf("camera_eye_x", 1.0f), conf->getf("camera_eye_y", 1.0f), conf->getf("camera_eye_z", 1.0f));

	v3 target = v3(conf->getf("camera_target_x"), conf->getf("camera_target_y"), conf->getf("camera_target_z"));

	direction = target-eye;
	direction.normalize();

	accSpeed = conf->getf("camera_acc", 50);
	brakeSpeed = conf->getf("camera_brakes", 200);

	moveSpeed.loadZero();

	if(conf->find("script") == NULL)
	{
		error("game res", "Cannot find script to init scene");
		return false;
	}

	world = new GameGroup();
	world->setProcessKind(PROCESS_NONE);
	
//	Object::global, 
	world->conf = conf->gets("script");
	if(!world->load(world)) return false;

// ******************* PHYSIC **************************
	physicHelper  = new GraphHelper;

//	physicEngine = new PhysicEngine3D;
	physicEngine = new PhysicEngineSteel;

	if(!physicEngine->init(conf->find("physic"))) return 1;
	this->bindPhysicEngine();

	g = conf->getv3("g");
	physicEngine->setGravitation(g);

// * crosshair
//	crosshair = new Sprite;
	
	ScriptLine csc;
	csc.set(conf->gets("crosshairConfig"));

/*	if(!crosshair->init(csc))
	{
		return false;
	}
*/
	_alive = true;
	paused = conf->geti("paused", 0) == 1;
	framesToPass = 0;
	speedup = 1;
	light = NULL;
/**
	Combiner *obj = new Combiner;
	obj->setGraphObject(new Sphere);
	AudioSourceRes *audio = new AudioSourceRes;
	audio->setSound(resAudio.add("audio/rain"));
	audio->setLooped(true);
	audio->setGain(1.0f);
	audio->setPitch(1.0f);
	//audio->setRolloff(0.0f);
	obj->setAudioObject(audio);
	obj->setPositionKind(POSITION_GLOBAL);
	obj->setPosition(
		matrix34::CreateTranslationMatrix(v3(0, 0, 10.0f))
		*matrix34::CreateRotationMatrix(1, v3(1,0,0))*0.5
		);
	world->addChildren(obj);
/**/
	Combiner* audioAm = new Combiner;
	AudioSourceRes *audioX = new AudioSourceRes();
	audioX->setSound(resAudio.add("audio/rain"));
	audioX->setLooped(true);
	audioX->setGain(0.1f);
	audioX->setPitch(1.0f);
	audioX->setLooped(true);
	//audioX->setRolloff(0.0f);
	audioX->setSourceRelative(true);
	audioAm->setAudioObject(audioX);
	audioAm->setPositionKind(POSITION_GLOBAL);
	audioAm->setPosition(
		matrix34::CreateTranslationMatrix(v3(0, 0, 0.0f))
		*matrix34::CreateRotationMatrix(1, v3(1,0,0))*0.5
		);
	world->addChildren(audioAm);

/**/
	Combiner *obj2 = new Combiner;
	obj2->setGraphObject(new Sphere);
	AudioSourceRes *audio2 = new AudioSourceRes;
	audio2->setSound(resAudio.add("audio/thunder"));
	audio2->setLooped(true);
	obj2->setAudioObject(audio2);
	obj2->setPositionKind(POSITION_GLOBAL);
	obj2->setPosition(
		matrix34::CreateTranslationMatrix(v3(10, 10, 1.0f))
		*matrix34::CreateRotationMatrix(1, v3(1,0,0))*0.5
		);
	world->addChildren(obj2);

	/**/
	Combiner *obj3= new Combiner;
	obj3->setGraphObject(new Sphere);
	AudioSourceRes *audio3 = new AudioSourceRes;
	audio3->setSound(resAudio.add("audio/bark"));
	audio3->setLooped(true);
	//audio3->setSound(audio2->getSound());
	//audio3->setLooped(true);
	obj3->setAudioObject(audio3);
	obj3->setPositionKind(POSITION_GLOBAL);
	obj3->setPosition(
		matrix34::CreateTranslationMatrix(v3(-10, -10, 1.0f))
		*matrix34::CreateRotationMatrix(1, v3(1,0,0))*0.5
		);
	world->addChildren(obj3);

	/**/

	Combiner *obj4= new Combiner;
	obj4->setGraphObject(new Sphere);
	AudioSourceRes *audio4 = new AudioSourceRes;
	audio4->setSound(resAudio.add("audio/test4"));
	audio4->setLooped(true);
	audio4->setRolloff(0.0f);
	obj4->setAudioObject(audio4);
	obj4->setPositionKind(POSITION_GLOBAL);
	obj4->setPosition(
		matrix34::CreateTranslationMatrix(v3(-10, 10, 1.0f))
		*matrix34::CreateRotationMatrix(1, v3(1,0,0))*0.5
		);
	world->addChildren(obj4);

	Combiner *obj5= new Combiner;
	obj5->setGraphObject(new Sphere);
	AudioSourceRes *audio5 = new AudioSourceRes;
	audio5->setSound(resAudio.add("audio/tank"));
	audio5->setLooped(true);
	
	obj5->setAudioObject(audio5);
	obj5->setPositionKind(POSITION_GLOBAL);
	obj5->setPosition(
		matrix34::CreateTranslationMatrix(v3(10, -10, 1.0f))
		*matrix34::CreateRotationMatrix(1, v3(1,0,0))*0.5
		);
	world->addChildren(obj5);

	light = new GameLight;
	light->setProcessKind(PROCESS_NONE);
	matrix34 m;		m.loadIdentity();		m.setTranslation(eye);		light->setPosition(m);
	world->addChildren(light);

	return true;
}


void Steel::handleEventKeyDown(std::string key)
{
	if(key == "escape") _alive = false;
	if(key == "pause") paused = !paused;
	if(key == "n") framesToPass = 1;

	if(key == "f1") physicEngine->conf->toggle("helperDrawLines");
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

		physicEngine->setGravitation(g);
	}

	if(key == "mouse1" && !conf->getf("automaticGun"))
		createObject();
}

bool Steel::createObject()
{
/*	Particle *p = new Particle;

	if(p->init(eye, direction, resOldConfig.add( "ps_weapon")))
	{
		graphEngine->inject(p);
		physicEngine->inject(p);
	}
*/
	if(!light)
	{
		light = new GameLight;
		light->setProcessKind(PROCESS_NONE);

		matrix34 m;		m.loadIdentity();		m.setTranslation(eye);		light->setPosition(m);

		graphEngine->inject(light);
	}
	else
	{
		matrix34 m;		m.loadIdentity();		m.setTranslation(eye);		light->setPosition(m);
	}

	return true;

	// not used, Kane
	//static int safe = 0;

	if(conf->getf("weaponSpeed")<EPSILON) return false;

	ScriptLine line;
	line.set(conf->gets("weapon"));
	GameObj *o = new GameObjModel;
	o->init(line);
	
	o->setProcessKind(PROCESS_UNI);

	matrix34 m = o->getPosition();

	v2 d(direction.x, direction.y);
	d.normalize();

	matrix34 rm1; 	rm1.loadIdentity();
	
//			rm1.setRotationZ(d.x, d.y);
	// not used, Kane
	//float cen = direction.z;

	//rm1.setRotationEuler(1,0, cen, sqrt(1-cen*cen), d.x, d.y);

	matrix34 pos = o->getPosition();
	
	m = m*rm1;
	m.setTranslation(pos.getTranslation().x*direction.getNormalized() + eye);

	o->setPosition(m);

	velocity v(o->getVelocity());
	v.translation = direction*conf->getf("weaponSpeed") /*+ cameraSpeed*/;
	v.rotationAxis = v3(0,0,0);
	o->setVelocity(v);

	return false;

/*	if(physicEngine->checkInvariant(*o, *o)
	{
		GameLight *light = NULL;
		Sprite *c = NULL;
		if(super)
		{
			lightTag = c;
			light = new GameLight;
			light->setPositionKind(POSITION_LOCAL);
			light->setProcessKind(PROCESS_NONE);
			o->addChildren(light);

			light->setPosition(matrix34::getIdentity());

			c = new Sprite;
			ScriptLine csc;
			csc.set(conf->gets("lightSprite"));
			c->init(csc);
			c->setPositionKind(POSITION_LOCAL);
			o->addChildren(c);
		}


		graphEngine->inject(o);
		physicEngine->inject(o);
		return true;
	}
	else
	{
		safe--;
		delete o;
		return false;
	}*/
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
 		physicEngine->process(totalPhysicTime, frame);
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

	graphEngine->inject(physicHelper);

	int count = world->getGraphChildrenCount();
	for(int i = 0; i < count; i++)
	{
		engine->inject(world->getGraphChildren(i));
	}
}

void Steel::bindPhysicEngine()
{
//	if(conf->geti("drawHelper"))
	physicEngine->bindHelper(physicHelper);

	int count = world->getPhysicChildrenCount();
	for(int i = 0; i < count; i++)
	{
		physicEngine->inject(world->getPhysicChildren(i));
	}
}

void Steel::insonify(AudioEngine *engine)
{
	Listener listener;
	listener.setPosition(eye.x, eye.y, eye.z);
	listener.setOrientation(v3(direction.x, direction.y, direction.z), v3(0,0,1));
	engine->setListener(listener);
}

void Steel::draw(GraphEngine *graph)
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

/*	if(conf->geti("crosshair"))
	{
		matrix34 m;

		m.loadIdentity(); m.setTranslation(eye + direction*0.01f);

		crosshair->setPosition(m);
		((Sprite*)crosshair)->setAlign(-direction);

		graph->inject(crosshair);
	}*/
	
	graph->camera.setup(eye, direction);
	graph->processCamera();
	graph->process(0,0);
	

//	if(conf->geti("crosshair"))
//		graph->remove(crosshair);
}



void Steel::deinit()
{
	if(physicEngine)
	{
		physicEngine->deinit();
		delete physicEngine;
		physicEngine = NULL;
	}
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


void Steel::bindAudioEngine(AudioEngine *engine)
{
	audioEngine = engine;
	audioEngine->setListenerEnvironment(EAX_ENVIRONMENT_GENERIC);
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

	int count = world->getAudioChildrenCount();
	for(int i = 0; i < count; i++)
	{
		engine->inject(world->getAudioChildren(i));
	}
}
