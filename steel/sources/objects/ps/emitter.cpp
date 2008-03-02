/*id*********************************************************
	File: objects/ps/emitter.cpp
	Unit: particle_system
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
	License:
		Steel Engine License
	Description:
		Инициализация и утилизация частиц в системе
************************************************************/
#include "emitter.h"
#include "../../common/utils.h"
#include "../../res/config/config.h"
#include "../../common/logger.h"
#include "../../engine/engine_types.h"


void SimpleEmitter::born(Particle& particle, int index)
{
	particle.velocity.loadZero();

    switch (type)
    {
        case EMITTER_RANDOM_CUBE:
        case EMITTER_SERIAL:
        {
            particle.position = this->position + v3(prand(), prand(), prand())*emitter_size;
            break;
        }

        case EMITTER_RANDOM_SPHERE:
        case EMITTER_RANDOM_FILLED_SPHERE:
        {
            float z = prand();
            float t = frand()*float(M_PI)*2.0f;
            float r = sqrt(1.0f-sqr(2.0f*z))*0.5f;
            float x = cos(t)*r;
            float y = sin(t)*r;
            v3 radius_vector = v3(x, y, z);

            if (type == EMITTER_RANDOM_FILLED_SPHERE)
            {
                radius_vector *= pow(frand(), 1.0f/3.0f);
            }
            particle.position = this->position + radius_vector*emitter_size;

            break;
        }

                //v3 a;
                //for (;;)
                //{
                //    a = prand_v3();
                //    if (a.getSquaredLength() <= 0.5*0.5)
                //    {
                //        break;
                //    }
                //}

                //particle.position = this->position + a*emitter_size;
        case EMITTER_UNIFORM_CUBE:
        {
            float w = pow(float(init_size), 1.0f/3.0f);
            int w2 = int(ceil(w-EPSILON));
            int x = index%w2;
            int y = index/w2%w2;
            int z = index/w2/w2;
            particle.position = this->position + v3(x/w, y/w, z/w)*emitter_size;
            break;
        }

        default:
            assert(false, "");
    }

    v3 radius_vector = particle.position - this->position;
//    particle.velocity = radius_vector.crossProduct(prand_v3()).getNormalized()*sqrt(radius_vector.getLength());


	particle.size = particle_size;
	particleSystem->particleBorn(index);
}


void SimpleEmitter::process(IN const ProcessInfo& info)
{
	if (frand() < 0.0f && set->particles.size() > 1) // delete particle
	{
		int dieId = rand()%set->particles.size(); // particle number

		delete set->particles[dieId];

		if ((size_t)(dieId + 1) < set->particles.size())
        {
			set->particles[dieId] = set->particles.back();
        }
		set->particles.pop_back();
	}

	if (type == EMITTER_SERIAL && last_born_time + born_interval < info.timeInfo.currentTime && set->particles.size() < limit_count)
	{
		int bornId = set->particles.size();
		set->particles.resize(bornId + 1);

		set->particles[bornId] = new Particle;

		born(*set->particles[bornId], bornId);

		last_born_time = info.timeInfo.currentTime;
	}
}

bool SimpleEmitter::InitFromConfig(Config& _conf)
{
	conf = &_conf;
	position = conf->getv3("origin");
    emitter_size = conf->getf("emitter_size", 1.0f);
    particle_size = conf->getf("particle_size", 1.0f);
    born_interval = conf->getf("born_interval", 1.0f);
    limit_count = conf->geti("limit_count", 1000);
	
	last_born_time = 0;

    std::string stype = conf->gets("emitter_type");
    init_size = (int)ceil(conf->geti("init_size") * set->countScale);

         if (stype == "random_cube") type = EMITTER_RANDOM_CUBE;
    else if (stype == "random_sphere") type = EMITTER_RANDOM_SPHERE;
    else if (stype == "random_filled_sphere") type = EMITTER_RANDOM_FILLED_SPHERE;
    else if (stype == "uniform_cube") type = EMITTER_UNIFORM_CUBE;
    else if (stype == "serial")			type = EMITTER_SERIAL;
    else type = EMITTER_RANDOM_CUBE;

	return true;
}
