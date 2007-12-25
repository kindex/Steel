/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Labyrinth Game main unit
 ************************************************************/

#include "character.h"
#include <engine/engine.h>
#include <objects/game_object_factory.h>
#include <graph/graph_interface.h>

Character::Character():
    graph_object(NULL),
    position(matrix34::getIdentity())
{}


bool Character::supportsInterface(IN OUT Engine&, IN const InterfaceId id)
{
    return id == INTERFACE_GRAPH;
}

bool Character::updateInformation(IN OUT Engine& engine, IN const InterfaceId id)
{
    if (id == INTERFACE_GRAPH)
    {
        dynamic_cast<GraphInterface*>(&engine)->setPosition(position);
        return true;
    }
    return false;
}

void Character::process(const ProcessInfo& info)
{
    position.setTranslation(position.getTranslation() + v3(1, 1, 0)*info.timeInfo.frameLength);
}

bool Character::InitFromConfig(Config& conf)
{
    graph_object = createGameObject(conf.find("graph"));
    health = conf.getf("health", 100);
    return graph_object != NULL;
}

void Character::bindEngine(IN OUT Engine& engine, IN const InterfaceId id)
{
    if (graph_object != NULL && id == INTERFACE_GRAPH)
    {
        dynamic_cast<ChildrenInterface*>(&engine)->addChild(graph_object);
    }
}
