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
			d.Normalize();
			eye -= (float)moveSpeed*(float)speed*d;
		}
		if(input->isPressed("d"))
		{
			v3 d(direction.y, -direction.x, 0);
			d.Normalize();
			eye += (float)moveSpeed*(float)speed*d;
		}
	}
}

void Game::handleMouse(double dx, double dy)
{
	angle.x += (float)dx;
	angle.y += (float)dy;
	if(angle.y >  M_PI*0.45f) angle.y =  (float)M_PI*0.45f;
	if(angle.y < -M_PI*0.45f) angle.y = -(float)M_PI*0.45f;
}

void Game::processPhysic(PhysicEngine *physic)
{
/*		for(int i=0; i<3; i++)
			obj[1]->pos.entries[i+i*4] = 0.20f;
*/

/*		light->position.x = (float)cos(time)*10.0f;
		light->position.y = (float)sin(time)*10.0f;
		light->position.z = 5.0f;
*/

//		gobj[1]->pos.entries[14] = 20;

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
	angle = v3(conf->getf("camera.angle.x"), conf->getf("camera.angle.y"), conf->getf("camera.angle.z"));

	direction = v3(-1.0f/eye.x, -1.0f/eye.y, -1.0f/eye.z);
	direction.Normalize();

	moveSpeed = conf->getd("camera.speed", 1);

	if(!conf->isset("script"))
	{
		alog.msg("error game res", "Cannot find script to init scene");
		return false;
	}
	string script = conf->gets("script");
	Script *s = (Script*)res->add(Res::script, script);
	if(!s)
	{
		alog.msg("error game res", "Cannot load script");
		return false;
	}
	alog.msg("game script", IntToStr(s->count()) + " Lines");
	for(int i=0; i<s->count(); i++)
	{
		GameObj *obj;

		if(s->count(i)<4) continue;
		string kind		= s->gets(i, 0);
		string parent	= s->gets(i, 1);
		string id		= s->gets(i, 2);
		string model	= s->gets(i, 3);

		bool gameobj = false, g = false, p = false;

		if(kind == "light")
		{
			obj = new GameLight;
			
			gameobj = true;
			g = true;
		}

		if(kind == "g" || kind == "solid" || kind == "f")
		{
			if(!res->add(Res::model, model)) return false;

			obj = new GameObjModel((Model*)res->getModel(model));

			obj->setMovable(kind != "solid");
			obj->setRotatable(kind != "solid");

			g = true;
			if(kind == "f" || kind == "solid")
				p = true;

			gameobj = true;
		}
		if(kind == "tag")
		{
			obj = new GameObjDummy();
			g = true;
			p = true;
			gameobj = true;
		}

		if(kind == "path")
		{
			obj = new GamePath();
			
			((GamePath*)obj)->setSpeed(s->getf(i, 3));

			obj->setPosition(tag[s->gets(i, 4)]->getPosition());

			for(int j=4; j<s->count(i); j++)
				((GamePath*)obj)->addTarget(s->gets(i, j));

			g = true;
			p = true;
		}

		if(id != "" && obj)
		{
			tag[id] = obj;
			obj->setName(id);
		}

		if(gameobj)	
			obj->setPosition(v3( s->getf(i, 4), s->getf(i, 5), s->getf(i, 6)));

		if(parent == "")
		{
			if(g)	gobj.push_back(obj);
			if(p)	pobj.push_back(obj);
			alog.msg("game script", string("Added object '") + kind + ":" + id + "' to global space");
		}
		else
		{
			if(tag.find(parent) == tag.end())
			{
				alog.msg("error game script", string("Object with id '") + parent + "' not found");
				return false;
			}
			tag[parent]->addChildren(obj);
			alog.msg("game script", string("Added object '") + kind + ":" + id + "' to '" + parent + "'");
		}
	}

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

