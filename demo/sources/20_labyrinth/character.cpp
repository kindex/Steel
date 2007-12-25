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

void Character::handleEventKeyDown(const std::string& key)
{
    v3 dir = zero;
    if (key == "w") dir += v3(+1,  0, 0);
    if (key == "s") dir += v3(-1,  0, 0);
    if (key == "a") dir += v3( 0, +1, 0);
    if (key == "d") dir += v3( 0, -1, 0);

    position.setTranslation(position.getTranslation() + dir);
}

v3 Character::getPosition() const
{
    return position.getTranslation();
}
