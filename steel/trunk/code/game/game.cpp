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

#include "objects/model_obj.h"

using namespace std;

void Game::handleEventKeyDown(std::string key)
{
	if(key == "escape") _alive = false;
	if(key == "pause") paused = !paused;
	if(key == "n") framesToPass = 1;

	if(key == "f1") conf->toggle("drawHelper");
	if(key == "f2") graphEngine->conf->toggle("drawTexture");
	if(key == "f3") graphEngine->conf->toggle("drawWire");
	if(key == "f4") graphEngine->conf->toggle("drawAABB");
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

/*	if(key == "mouse1" && input->isMouseCaptured())
		createObject();
		*/
}

bool Game::createObject()
{
	static int balls = 0;

	ScriptLine line;
	line.set(conf->gets("weapon"));
	GameObj *o = new GameObjModel;
	o->init(line, *res);
	
	o->changePositionKind(Interface::global);
	o->setProcessKind(ProcessKind::uni);

	matrix44 m = o->getMatrix();

	v2 d(direction.x, direction.y);
	d.normalize();

	matrix44 rm1; 	rm1.loadIdentity();
	
//			rm1.setRotationZ(d.x, d.y);
	float cen = direction.z;

	rm1.setRotationEuler(1,0, cen, sqrt(1-cen*cen), d.x, d.y);

	v3 pos = o->getPosition();
	
	m = m*rm1;
	m.setTranslation(pos.x*direction.getNormalized() + eye);

	o->setMatrix(m);

	velocity v(o->getVelocity());
	v.translation = direction*v.translation.x + cameraSpeed;
	v.rotationAxis = v3(0,0,0);
	o->setVelocity(v);

	GameLight *light = NULL;
	Sprite *c = NULL;
	if(balls == 0)
	{
		light = new GameLight;
		light->changePositionKind(Interface::local);
		light->setProcessKind(ProcessKind::none);
		o->addChildren(light);
		matrix44 m;
		m.loadIdentity();
		light->setMatrix(m);

		c = new Sprite;
		ScriptLine csc;
		csc.set(conf->gets("lightSprite"));
		c->init(csc, *res);
		c->changePositionKind(Interface::local);
		o->addChildren(c);

		m.setTranslation(pos.x*direction.getNormalized() + eye);
		o->setMatrix(m);
	}

	if(physicEngine->checkInvariant(*o, *o))
	{
		world->addChildren(o);
		graphEngine->inject(o);
		physicEngine->inject(o);
		balls++;
		return true;
	}
	else
	{
		if(light) delete light;
		if(c) delete c;
		delete o;
		return false;
	}
}

void Game::processKeyboard()
{
	if(input->isMouseCaptured())
	{
		if(input->isPressed("mouse1"))
		{
			createObject();
		}


		v3 dir(0,0,0);
		if(input->isPressed("w")) 	dir += v3(1,0,0);
		if(input->isPressed("s")) 	dir += v3(-1,0,0);
		if(input->isPressed("a")) 	dir += v3(0, +1, 0);
		if(input->isPressed("d")) 	dir += v3(0, -1, 0);
		if(input->isPressed("'")) 	dir += v3(0, 0, +1);
		if(input->isPressed("/")) 	dir += v3(0, 0, -1);

		if(dir.getLength()>EPSILON)
			moveSpeed += dir*accSpeed*(float)speed;
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

	if(conf->geti("drawHelper"))
		graph->inject(physicHelper);
	if(conf->geti("crosshair"))
	{
		crosshair->setPosition(eye + direction*0.01f);
		((Sprite*)crosshair)->setAlign(-direction);

		graph->inject(crosshair);
	}

	graph->camera.setup(eye, direction);
	graph->processCamera();
	graph->process();

	if(conf->geti("drawHelper"))
		graph->remove(physicHelper);
	if(conf->geti("crosshair"))
		graph->remove(crosshair);
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
	world->conf = conf->gets("script");
	if(!world->load(res, world)) return false;

// ******************* PHYSIC **************************
	physicHelper  = new GraphHelper;

	physicEngine = new PhysicEngine3D;
	physicEngine->bindResColelntion(res);
	if(!physicEngine->init("physic")) return 1;
	this->bindPhysicEngine();

	g = conf->getv3("g");
	physicEngine->setGravitation(g);

// * crosshair
	crosshair = new Sprite;
	
	ScriptLine csc;
	csc.set(conf->gets("crosshairConfig"));

	if(!crosshair->init(csc, *res))
	{
		return false;
	}

	_alive = true;
	paused = conf->geti("paused", 0) == 1;
	framesToPass = 0;
	speedup = 1;

	return true;
}

void Game::handleEventKeyUp(std::string key)
{
}

bool Game::executeScript(std::string script)
{
	alog.msg("console", "ExecScript: '" + script + "'");

	vector<string> lines = explode(';', script);
	for(vector<string>::const_iterator it = lines.begin(); it != lines.end(); it++)
		if(!executeCommand(*it)) return false;
	return true;
}

bool Game::executeCommand(std::string command)
{
	if(command.empty()) return true;
	alog.msg("console", "ExecCommand: '" + command + "'");

	// var=value

	vector<string> token = explode('=', command);
	if(token.size() != 2) return false;
	vector<string> var = explode('.', token[0]);
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

