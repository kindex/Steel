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
#include <objects/combiner/graph_object.h>
#include <objects/ps/emitter.h>
#include <graph/graph_interface.h>
#include <input/input.h>
#include <res/res_main.h>
#include <NxPhysics.h>
#include "steel_nx.h"

Character::Character():
    graph_object(NULL),
    physic_object(NULL),
    origin(matrix34::getIdentity()),
    input(NULL),
    direction(v3(1, 0, 0)),
    owner(FREE),
    clientId(0),
    position_is_set(false),
    smoke(NULL),
    state(IDLE)
{}

bool Character::supportsInterface(IN OUT Engine&, IN const InterfaceId id)
{
    return id == INTERFACE_GRAPH;
}

bool Character::updateInformation(IN OUT Engine& engine, IN const InterfaceId id)
{
    if (id == INTERFACE_GRAPH)
    {
        GraphInterface& gengine = *dynamic_cast<GraphInterface*>(&engine);

        gengine.setPositionKind(POSITION_GLOBAL);
        gengine.setPosition(getPosition());

        if (!name.empty())
        {
            float text_size = 0.15f;
            GraphText text(name, 
                           ObjectPosition::createTranslationMatrix(v3(0, 0, 0.35f)),
                           POSITION_LOCAL,
                           v2(text_size, text_size),
                           SPRITE_ALIGN_SCREEN,
                           GraphText::ALIGN_CENTER,
                           resFont.add("/font/arial"),
                           color4f(1, 0, 0.5f));
            GraphTextVector tv;
            tv.push_back(text);
            gengine.setGraphText(tv);
        }

        return true;
    }
    return false;
}

void Character::jump()
{
		//if (input->isPressed("j")){
		//	if(jumpinprogress != true)
		//	{
		//		// check if character collide with a wall
		//		//if(physic_object->isDynamic() || 
		//		//(a->getActor()->isDynamic() || b->getActor()->isDynamic())        && NxScene::getGroupCollisionFlag(a->getGroup(), b->getGroup())        && (!(NxScene::getShapePairFlags(a,b) & NX_IGNORE_PAIR))

		//		jump = true;
		//		//jumpinprogress = true;
		//	}
		//}
		//else {
		//	jump = false;
		//	jumpinprogress = false;
		//}

	NxVec3 vecJump(0, 0, 20);
	physic_object->addForce(vecJump);
}

void Character::process(const ProcessInfo& info)
{
	static bool jump = false;
	static bool jumpinprogress = false;
    if (smoke != NULL)
    {
        smoke->emitter->setPosition(getPosition().getTranslation() + v3(0, 0, 0.4f));
        smoke->process(info);
    }
    if (input != NULL)
    {
        v3 dir = zero;
        if (input->isPressed("w")) dir += v3(+1,  0, 0);
        if (input->isPressed("s")) dir += v3(-1,  0, 0);
        if (input->isPressed("a")) dir += v3( 0, +1, 0);
        if (input->isPressed("d")) dir += v3( 0, -1, 0);
        if (dir.getSquaredLength() != 0 && physic_object != NULL)
        {
            v3 direction2 = direction;
            direction2.z = 0;
            direction2.getNormalized();
	        v3 forceDirection = dir.x*direction2
			        + dir.y*v3(-direction2.y, direction2.x, 0).getNormalized()
			        + dir.z*v3(0, 0, 1);

            forceDirection.normalize();
            NxReal gForceStrength = force*info.timeInfo.frameLength;

            NxVec3 old_vec = physic_object->getLinearVelocity();
            physic_object->setLinearVelocity(old_vec + gForceStrength*tonx(forceDirection));
            state = MOVING;
        }
        else
        {
            state = IDLE;
        }
    }
}

bool Character::InitFromConfig(Config& conf)
{
    assert(false);

    return false;
}

Config* Character::getConfig() const
{
    ConfigStruct* result = new ConfigStruct;

	result->setValue("class", new ConfigString("character"));
    result->setValue("origin", createV3config(origin.getTranslation()));

    return result;
}

void Character::bindEngine(IN OUT Engine& engine, IN const InterfaceId id)
{
    if (graph_object != NULL && id == INTERFACE_GRAPH)
    {
        dynamic_cast<ChildrenInterface*>(&engine)->addChild(graph_object);
        if (smoke != NULL)
        {
            dynamic_cast<ChildrenInterface*>(&engine)->addChild(smoke);
        }
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

v3 Character::getAngularMomentum() const
{
    return tov3(physic_object->getAngularMomentum());
}

bool Character::trustPosition(const CharacterPosition& pos) const
{
    return (pos.position.getTranslation() - getPosition().getTranslation()).getLength() < trust_distance;
}

void Character::setCharacterPosition(const CharacterPosition& pos)
{
    position_is_set = true;
    setPosition(pos.position);
    setVelocity(pos.linear_velocity);
    setAngularMomentum(pos.linear_momentum);
}

void Character::setPosition(const ObjectPosition& new_position)
{
    if (physic_object != NULL)
    {
        physic_object->setGlobalPose(tonx(new_position));
    }
    else
    {
        origin = new_position;
    }
}

void Character::setVelocity(const v3& v)
{
    physic_object->setLinearVelocity(tonx(v));
}

void Character::setAngularMomentum(const v3& v)
{
    physic_object->setAngularMomentum(tonx(v));
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

