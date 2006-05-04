#include "particle_system.h"
#include "../../common/utils.h"

using namespace std;


bool ParticleSystem::init(ScriptLine &s, ResCollection &_res)
{
	if(!GameObj::init(s, _res)) return false;

	if(s.count()<3) return false;
	res = &_res;
	conf = (Config*)res->add(Res::config, s.gets(3));

	int cnt = conf->geti("count", 1);

	m = (Material*)res->add(Res::material, conf->gets("material"));
	if(!m) return false;

	sprite.resize(cnt);
	particle.resize(cnt);

	for(int i=0; i<cnt; i++)
	{
		particle[i].alive = false;
	}
	zedAlign = true;
	initSprites();

	return true;
}


void ParticleSystem::process(steel::time curTime, steel::time frameLength, PhysicEngine *engine)
{
	int cnt = particle.size();

	int limit = (int)(((float)frameLength * (float)conf->geti("count")/((conf->getf("maxlifetime")+	conf->getf("minlifetime"))*0.5f) + 1));

	for(int i=0; i<cnt; i++)
	{
		if(particle[i].alive)
			particle[i].process(curTime, frameLength);
		else
			if(limit>0)
			{
				particle[i].born(curTime, frameLength, conf, getParent()->getGlobalMatrix(), getParent()->getGlobalVelocity());
				limit--;
			}
	}
}

void Particle::process(steel::time curTime, steel::time frameLength)
{
	velocity -= velocity*speedDown*frameLength;

/*	if(velDown>1) velDown = 1;
	if(velDown<0) velDown = 0;
	velocity *= (1 - velDown);*/

	velocity += acc*(float)frameLength;

	position += velocity * (float)frameLength;
	if(endTime< curTime) alive = false;
}

void Particle::born(steel::time curTime, steel::time frameLength, Config *conf, matrix44 global, v3 globalVelocity)
{
	v3	dir = conf->getv3("direction");

	dir.rotateZ(conf->getf("angle1")*G_PI * prand());
	dir.rotateAxis(conf->getf("angle2")*G_PI * prand(), v3( -dir.y, dir.x, 0));

	v3	sp = dir*(conf->getf("minspeed") + frand()*(conf->getf("maxspeed") - conf->getf("minspeed")));

	v3 initPosDelta = conf->getv3("initPosDelta");
	position = global * v3(prand()*initPosDelta.x, prand()*initPosDelta.y, prand()*initPosDelta.z);
	
	velocity = globalVelocity * conf->getf("parentSpeedK")  + sp;

	startTime = (float)curTime;
	endTime = (float)curTime + conf->getf("minlifetime") + frand()*(conf->getf("maxlifetime") - conf->getf("minlifetime"));
	size = conf->getf("size");

	acc = conf->getv3("acc");
	speedDown = conf->getf("speedDown");
	alive = true;
}

void ParticleSystem::processGraph(v3	cameraEye, v3 cameraDirection)
{
	int cnt = particle.size();

	for(int i=0; i<cnt; i++)
	{
		sprite[i].pos = particle[i].position;
		sprite[i].size = particle[i].alive?particle[i].size:0;
	}

	SpriteSystem::processGraph(cameraEye, cameraDirection);
}

