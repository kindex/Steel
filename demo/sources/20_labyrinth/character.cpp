/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2007
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
#include "steel_nx.h"

Character::Character():
    graph_object(NULL),
    physic_object(NULL),
    origin(matrix34::getIdentity()),
    input(NULL),
    direction(v3(1, 0, 0)),
    owner(FREE),
    clientId(0)
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
        dynamic_cast<GraphInterface*>(&engine)->setPosition(getPosition());

        GraphText text("Player", ObjectPosition::CreateTranslationMatrix(v3(0, 0, 0.35f)), POSITION_LOCAL, v2(0.05f, 0.03f), SPRITE_ALIGN_SCREEN);
        GraphTextVector tv;
        tv.push_back(text); // TODO: align, player name

        dynamic_cast<GraphInterface*>(&engine)->setGraphText(tv);

        return true;
    }
    return false;
}

void Character::process(const ProcessInfo& info)
{
    if (input != NULL)
    {
        v3 dir = zero;
        if (input->isPressed("w")) dir += v3(+1,  0, 0);
        if (input->isPressed("s")) dir += v3(-1,  0, 0);
        if (input->isPressed("a")) dir += v3( 0, +1, 0);
        if (input->isPressed("d")) dir += v3( 0, -1, 0);
        if (dir.getSquaredLength() != 0)
        {
            v3 direction2 = direction;
            direction2.z = 0;
            direction2.getNormalized();
	        v3 forceDirection = dir.x*direction2
			        + dir.y*v3(-direction2.y, direction2.x, 0).getNormalized()
			        + dir.z*v3(0, 0, 1);

            NxReal gForceStrength = force*info.timeInfo.frameLength;

            NxVec3 old_vec = physic_object->getLinearVelocity();
            physic_object->setLinearVelocity(old_vec + gForceStrength*tonx(forceDirection));
        }
    }
}

bool Character::InitFromConfig(Config& conf)
{
    graph_object = createGameObject(conf.find("graph"));
    health = conf.getf("health", 100);
    origin.setTranslation(conf.getv3("origin"));
    force = conf.getf("force", 10);

    return graph_object != NULL;
}

Config* Character::getConfig() const
{
    ConfigStruct* result = new ConfigStruct;

	result->setValue("class", new ConfigString("character"));
    result->setValue("origin", createV3config(origin.getTranslation()));
    result->setValue("force", new ConfigNumber(force));
    result->setValue("graph", graph_object->getConfig());

    return result;
}


void Character::bindEngine(IN OUT Engine& engine, IN const InterfaceId id)
{
    if (graph_object != NULL && id == INTERFACE_GRAPH)
    {
        dynamic_cast<ChildrenInterface*>(&engine)->addChild(graph_object);
    }
}

ObjectPosition Character::getPosition() const
{
    if (physic_object != NULL)
    {
        return to_matrix34(physic_object->getGlobalPose());
    }
    else
    {
        return origin;
    }
}

v3 Character::getVelocity() const
{
    return tov3(physic_object->getLinearVelocity());
}

v3 Character::getMomentum() const
{
    return tov3(physic_object->getLinearMomentum());
}

void Character::setPosition(const ObjectPosition& new_position)
{
    physic_object->setGlobalPose(tonx(new_position));
}

void Character::setVelocity(const v3& v)
{
    physic_object->setLinearVelocity(tonx(v));
}

void Character::setMomentum(const v3& v)
{
    physic_object->setLinearMomentum(tonx(v));
}

void Character::setDirection(const v3& dir)
{
    direction = dir;
}

void Character::setInput(Input* _input)
{
    input = _input;
}

void Character::traverse(Visitor& visitor, const ObjectPosition& base_position)
{
    if (visitor.visit(this))
    {
        ObjectPosition new_position = origin*base_position;

        if (graph_object != NULL)
        {
            graph_object->traverse(visitor, new_position);
        }
        visitor.postvisit(this, new_position);
    }
}
