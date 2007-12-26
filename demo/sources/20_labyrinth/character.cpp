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
#include <input/input.h>
#include <NxPhysics.h>

Character::Character():
    graph_object(NULL),
    position(matrix34::getIdentity()),
    origin(matrix34::getIdentity())
{}

bool Character::supportsInterface(IN OUT Engine&, IN const InterfaceId id)
{
    return id == INTERFACE_GRAPH;
}

bool Character::updateInformation(IN OUT Engine& engine, IN const InterfaceId id)
{
    if (id == INTERFACE_GRAPH)
    {
        dynamic_cast<GraphInterface*>(&engine)->setPositionKind(POSITION_GLOBAL);
        dynamic_cast<GraphInterface*>(&engine)->setPosition(position);
        return true;
    }
    return false;
}

NxVec3 ApplyForceToActor(NxActor* actor, const NxVec3& forceDir, const NxReal forceStrength, bool forceMode)
{
	NxVec3 forceVec = forceStrength*forceDir;

	if (forceMode)
    {
		actor->addForce(forceVec);
    }
	else
    {
		actor->addTorque(forceVec);
    }

	return forceVec;
}

void Character::process(const ProcessInfo& info)
{
    v3 dir = zero;
    if (input->isPressed("w")) dir += v3(+1,  0, 0);
    if (input->isPressed("s")) dir += v3(-1,  0, 0);
    if (input->isPressed("a")) dir += v3( 0, +1, 0);
    if (input->isPressed("d")) dir += v3( 0, -1, 0);

    //position.setTranslation(position.getTranslation() + dir);

    NxReal gForceStrength = 50*info.timeInfo.frameLength;
    bool bForceMode = true;

    NxVec3 gForceVec = ApplyForceToActor(physic_object, NxVec3(dir.x,dir.y, dir.z), gForceStrength, bForceMode);
}

bool Character::InitFromConfig(Config& conf)
{
    graph_object = createGameObject(conf.find("graph"));
    health = conf.getf("health", 100);
    origin.setTranslation(conf.getv3("origin"));

    return graph_object != NULL;
}

void Character::bindEngine(IN OUT Engine& engine, IN const InterfaceId id)
{
    if (graph_object != NULL && id == INTERFACE_GRAPH)
    {
        dynamic_cast<ChildrenInterface*>(&engine)->addChild(graph_object);
    }
}

const ObjectPosition& Character::getPosition() const
{
    return position;
}

void Character::setPosition(ObjectPosition& new_position)
{
    position = new_position;
}

void Character::traverse(Visitor& visitor, const ObjectPosition& base_position)
{
    if (visitor.visit(this))
    {
        ObjectPosition new_position = origin*position*base_position;

        if (graph_object != NULL)
        {
            graph_object->traverse(visitor, new_position);
        }
        visitor.postvisit(this, new_position);
    }
}