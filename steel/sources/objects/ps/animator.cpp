/*id*********************************************************
	File: objects/ps/animator.cpp
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Обрабатывает движение частиц в системе.
************************************************************/
#include "animator.h"
#include "../../res/config/config.h"
#include "../../engine/engine_types.h"

bool SimpleAnimator::initParticles()
{
	return true;
}

void SimpleAnimator::process(IN const ProcessInfo& info)
{
    for EACH(ParticleVector, set->particles, particle)
    {
        v3& velocity = (*particle)->velocity;
        v3& position = (*particle)->position;

        v3 local_velocity = velocity-wind;

        float force = pow(local_velocity.getLength(), friction_power)*friction_k;

        if (force > EPSILON)
        {
            v3 penalty = local_velocity*force*info.timeInfo.frameLength;
            if (local_velocity.getSquaredLength() < penalty.getSquaredLength())
            {
                velocity = wind;
            }
            else
            {
                velocity -= penalty;
            }
        }

        position += velocity*info.timeInfo.frameLength;
    }

    return;
}

bool SimpleAnimator::InitFromConfig(Config& conf)
{
    wind = conf.getv3("wind");
    friction_power = conf.getf("particle.friction_power", 2);
    friction_k = conf.getf("particle.friction_k", 1);

    return true;
}

bool SimpleAnimator::updateInformation(IN OUT Engine&, IN const InterfaceId id)
{
    return false;
}
