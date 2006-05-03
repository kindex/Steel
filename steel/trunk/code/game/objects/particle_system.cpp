#include "particle_system.h"
#include "../../common/utils.h"

using namespace std;


bool ParticleSystem::init(ScriptLine &s, ResCollection &_res)
{
	if(s.count()<3) return false;
	res = &_res;
	conf = (Config*)res->add(Res::config, s.gets(3));

	int cnt = conf->geti("count", 1);

	m = (Material*)res->add(Res::material, conf->gets("material"));
	if(!m) return false;

	float size = conf->getf("size");

	sprite.resize(cnt);
	particle.resize(cnt);

	for(int i=0; i<cnt; i++)
	{
		sprite[i].size = size;
		born(i);
	}
	initSprites();

	return true;
}


void ParticleSystem::process(steel::time curTime, steel::time frameLength, PhysicEngine *engine)
{
	int cnt = particle.size();

	for(int i=0; i<cnt; i++)
	{
		particle[i].position += particle[i].velocity * (float)frameLength;
		particle[i].lifetime -= (float)frameLength;
		if(particle[i].lifetime < 0)
			born(i);

	}
}


void ParticleSystem::born(int i)
{
	coord speed = conf->getf("speed"),		life = conf->getf("lifetime");

	matrix44 global = getParent()->getGlobalMatrix();
	v3 globalVelocity  = getParent()->getGlobalVelocity();

	particle[i].position = global * v3(0,0,0);
	particle[i].velocity = globalVelocity  + v3(speed*(frand()-0.5f), speed*(frand()-0.5f), speed*(frand()-0.5f));
	particle[i].lifetime = frand()*life;
}

void ParticleSystem::processGraph(v3	cameraEye)
{
	int cnt = particle.size();
	for(int i=0; i<cnt; i++)
		sprite[i].pos = particle[i].position;

	Sprites::processGraph(cameraEye);
}

