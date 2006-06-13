/*id*********************************************************
    Unit: game
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
		Этот юнит является дополнением к steel engine и служит
		примером использования движка. В этом юните задаются правила игры
		или графической демки.
 ************************************************************/

#include "game.h"
#include "../common/logger.h"
#include "../common/utils.h"

#include "../res/script/script.h"

#include "objects/model_obj.h"

using namespace std;

void Game::handleEventKeyDown(std::string key)
{
	if(key == "escape") _alive = false;
	if(key == "pause") paused = !paused;
	if(key == "n") framesToPass = 1;

	if(key == "f1") physicEngine->conf->toggle("helperDrawLines");
	if(key == "f2") graphEngine->conf->toggle("drawFill");
	if(key == "f3") graphEngine->conf->toggle("drawWire");
	if(key == "f4") graphEngine->conf->toggle("drawBump");
	if(key == "f5") graphEngine->conf->toggle("drawVertexes");

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

	if(key == "c") conf->toggle("crosshair");

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

bool Game::createObject()
{
/*	Particle *p = new Particle;

	if(p->init(eye, direction, (Config*)res->add(Res::config, "ps_weapon"), *res))
	{
		graphEngine->inject(p);
		physicEngine->inject(p);
	}
*/
	if(!light)
	{
		light = new GameLight;
		light->setPositionKind(PositionKind::global);
		light->setProcessKind(ProcessKind::none);

		matrix34 m;		m.loadIdentity();		m.setTranslation(eye);		light->setPosition(m);

		graphEngine->inject(light);
	}
	else
	{
		matrix34 m;		m.loadIdentity();		m.setTranslation(eye);		light->setPosition(m);
	}



	return true;

	static int safe = 0;

	if(conf->getf("weaponSpeed")<EPSILON) return false;

	ScriptLine line;
	line.set(conf->gets("weapon"));
	GameObj *o = new GameObjModel;
	o->init(line, *res);
	
	o->setPositionKind(PositionKind::global);
	o->setProcessKind(ProcessKind::uni);

	matrix34 m = o->getPosition();

	v2 d(direction.x, direction.y);
	d.normalize();

	matrix34 rm1; 	rm1.loadIdentity();
	
//			rm1.setRotationZ(d.x, d.y);
	float cen = direction.z;

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
			light->setPositionKind(PositionKind::local);
			light->setProcessKind(ProcessKind::none);
			o->addChildren(light);

			light->setPosition(matrix34::getIdentity());

			c = new Sprite;
			ScriptLine csc;
			csc.set(conf->gets("lightSprite"));
			c->init(csc, *res);
			c->setPositionKind(PositionKind::local);
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

void Game::processKeyboard()
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

void Game::handleMouse(double dx, double dy)
{
	direction.rotateZ((float)dx);

	if(dy<0 && direction.dotProduct(v3(0,0, 1))<0.99 || 
		dy>0 && direction.dotProduct(v3(0,0, -1))<0.99 )

	direction.rotateAxis((float)dy, v3( -direction.y, direction.x, 0));
}

void Game::process(steel::time globalTime, steel::time time)
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

void Game::bind(GraphEngine *engine)
{
	graphEngine = engine;

	graphEngine->inject(physicHelper);

	GraphInterfaceList list = world->getChildrens();
	for(GraphInterfaceList::iterator it = list.begin(); it != list.end(); it++)
	{
		engine->inject(*it);
	}

//	engine->inject(world);
}

void Game::bindPhysicEngine()
{
//	if(conf->geti("drawHelper"))
	physicEngine->bindHelper(physicHelper);

	PhysicInterfaceList list = world->getPChildrens();
	for(PhysicInterfaceList::iterator it = list.begin(); it != list.end(); it++)
	{
		physicEngine->inject(*it);
	}
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
	graph->process();

//	if(conf->geti("crosshair"))
//		graph->remove(crosshair);
}


bool Game::init(ResCollection *_res, string _conf, Input *_input, std::string params)
{
	// Config
	res = _res;	
	input = _input; 
	input->setGame(this);
	conf = (Config*)res->add(Res::config, _conf);
	if(!conf)
	{
		alog.msg("error game res", string("Cannot load game config ") + _conf);
		return false;
	}
	if(!executeScript(params))
	{
		alog.msg("core error", "Cannor execute script");
		return false;
	}

	// Init world
	eye = v3(conf->getf("camera_eye_x", 1.0f), conf->getf("camera_eye_y", 1.0f), conf->getf("camera_eye_z", 1.0f));

	v3 target = v3(conf->getf("camera_target_x"), conf->getf("camera_target_y"), conf->getf("camera_target_z"));

	direction = target-eye;
	direction.normalize();

	accSpeed = conf->getf("camera_acc", 50);
	brakeSpeed = conf->getf("camera_brakes", 200);

	moveSpeed.loadZero();

	if(!conf->isset("script"))
	{
		alog.msg("error game res", "Cannot find script to init scene");
		return false;
	}

	world = new GameGroup();
	world->setProcessKind(ProcessKind::none);
	
//	Interface::global, 
	world->conf = conf->gets("script");
	if(!world->load(res, world)) return false;

// ******************* PHYSIC **************************
	physicHelper  = new GraphHelper;

//	physicEngine = new PhysicEngine3D;
	physicEngine = new PhysicEnginePS;

	physicEngine->bindResColelntion(res);
	if(!physicEngine->init("physic")) return 1;
	this->bindPhysicEngine();

	g = conf->getv3("g");
	physicEngine->setGravitation(g);

// * crosshair
//	crosshair = new Sprite;
	
	ScriptLine csc;
	csc.set(conf->gets("crosshairConfig"));

/*	if(!crosshair->init(csc, *res))
	{
		return false;
	}
*/
	_alive = true;
	paused = conf->geti("paused", 0) == 1;
	framesToPass = 0;
	speedup = 1;
	light = NULL;

	return true;
}

void Game::handleEventKeyUp(std::string key)
{
}

bool Game::executeScript(std::string script)
{
	alog.msg("console", "ExecScript: '" + script + "'");

	steel::vector<string> lines = explode(';', script);
	for(steel::vector<string>::const_iterator it = lines.begin(); it != lines.end(); it++)
		if(!executeCommand(*it)) return false;
	return true;
}

bool Game::executeCommand(std::string command)
{
	if(command.empty()) return true;
	alog.msg("console", "ExecCommand: '" + command + "'");

	// var=value

	steel::vector<string> token = explode('=', command);
	if(token.size() != 2) return false;
	steel::vector<string> var = explode('.', token[0]);
	if(var.size() == 1)
	{
		conf->setup(var[0], token[1]);
		alog.msg("script", var[0] + " = " + token[1]);
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

