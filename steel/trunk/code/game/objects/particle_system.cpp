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

	int limit = frameLength * (float)conf->geti("count")/((conf->getf("maxlifetime")+	conf->getf("minlifetime"))*0.5f) + 1;

	for(int i=0; i<cnt; i++)
	{
		particle[i].position += particle[i].velocity * (float)frameLength;
		if(particle[i].endTime< curTime) particle[i].alive = false;

		if(!particle[i].alive && limit>0)
		{
			born(i, curTime, frameLength);
			limit--;
		}
	}
}


void ParticleSystem::born(int i, steel::time curTime, steel::time frameLength)
{
	v3	dir = conf->getv3("direction");

	dir.rotateZ(conf->getf("angle1")*G_PI * prand());
	dir.rotateAxis(conf->getf("angle2")*G_PI * prand(), v3( -dir.y, dir.x, 0));

	v3	sp = dir*(conf->getf("minspeed") + frand()*(conf->getf("maxspeed") - conf->getf("minspeed")));

	matrix44 global = getParent()->getGlobalMatrix();
	v3 globalVelocity  = getParent()->getGlobalVelocity();

	v3 initPosDelta = conf->getv3("initPosDelta");
	particle[i].position = global * v3(prand()*initPosDelta.x, prand()*initPosDelta.y, prand()*initPosDelta.z);
	
	particle[i].velocity = globalVelocity * conf->getf("parentSpeedK")  + sp;

	particle[i].startTime = (float)curTime;
	particle[i].endTime = (float)curTime + conf->getf("minlifetime") + frand()*(conf->getf("maxlifetime") - conf->getf("minlifetime"));
	particle[i].size = conf->getf("size");

	particle[i].alive = true;
}

void ParticleSystem::processGraph(v3	cameraEye)
{
	int cnt = particle.size();

	for(int i=0; i<cnt; i++)
	{
		sprite[i].pos = particle[i].position;
		sprite[i].size = particle[i].alive?particle[i].size:0;
	}

	SpriteSystem::processGraph(cameraEye);
}

