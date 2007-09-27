/*id*********************************************************
	Unit: Demo 19
	Part of: DiVision intro
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Pasticle system testtype
 ************************************************************/

#include "19_game.h"
#include <common/logger.h>
#include <engine/visitor.h>
#include <res/res_main.h>
#include <objects/ps/particle_system.h>
#include <objects/combiner/combiner.h>

GamePS::GamePS():
	physicEngine(NULL),
    boundingModel(NULL),
    crosses(0),
	gravityBackup(v3(0,0,0))
{}

bool GamePS::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}
	physicEngine = new PhysicEngine;

    Config* physicConfig = _conf.find("physic");
    if (physicConfig == NULL)
    {
        abort_init("game physic", "Cannot find physic config");
    }
	physicEngine->init(*physicConfig);

	physicEngine->inject(world);

    speedup = _conf.getf("speed", 1.0f);

    boundingModel = loadGraphObject(_conf, "boundingModel");

	return true;
}

bool isPointInObject(IN v3 point, IN const GraphObject& object, const ObjectPosition position)
{
    size_t crossCount = 0;
    
    Line lineSegment(point, v3(0.0, 0.0, 1.0));

    const Faces&        faces    = *object.getAllFaces();
    const VertexVector* vertexes = object.getVertexes();

	for EACH_CONST(TriangleVector, faces.triangles, it)
	{
		Plane triangle;
		triangle.base = position * vertexes->at(it->a[0]);
		triangle.a = position * vertexes->at(it->a[1]) - triangle.base;
		triangle.b = position * vertexes->at(it->a[2]) - triangle.base;
		float k;
		if (isCrossTrgLine(triangle, lineSegment, k))
		{
			if (k > 0.0)
			{
				crossCount++;
			}
		}
	}
	return crossCount%2 == 1;
}


class ParticleVisitor : public Visitor
{
public:
    ParticleVisitor(GraphObject* graphObject) : graphObject(graphObject) {}
        
    void clear()
    {
        cnt = 0;
    }

    void postvisit(IN OUT Particle* particle)
    {
        if (isPointInObject(particle->position, *graphObject, ObjectPosition::getIdentity()))
        {
            cnt++;
        }
    }

    int cnt;
    GraphObject* graphObject;
};

void GamePS::process()
{
	GameFreeScene::process();
	
	if (timeInfo.frameLength > EPSILON)
	{
        physicEngine->setSpeedup(speedup);
		physicEngine->process(timeInfo);
	}

    if (boundingModel != NULL)
    {
        ParticleVisitor visitor(boundingModel);

        visitor.clear();
        world->traverse(visitor);

        crosses = visitor.cnt;
    }
}

void GamePS::handleEventKeyDown(const std::string& key)
{
	if (key == "1") speedup = 0.01f;
	else if (key == "2") speedup = 0.05f;
	else if (key == "3") speedup = 0.2f;
	else if (key == "4") speedup = 0.5f;
	else if (key == "5") speedup = 1;
	else if (key == "6") speedup = 2;
	else if (key == "7") speedup = 5;
	else if (key == "8") speedup = 20;
	else if (key == "9") speedup = 50;
	else if (key == "-") speedup /= 2;
	else if (key == "+") speedup *= 2;
	else if (key == "g")
	{
		v3 current = physicEngine->getGravity();
		physicEngine->setGravity(gravityBackup);
		gravityBackup = current;
	}
    else
    {
        GameFreeScene::handleEventKeyDown(key);
    }
}

std::string GamePS::getWindowCaption()
{
    return caption + " Crosses: " + IntToStr(crosses);
}
