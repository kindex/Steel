#include "particle_system.h"

using namespace std;

//Returns random value [0..1)
float frand()
{
	return (float) rand() / (float) RAND_MAX;
}


Sprites*	GameParticleSystem::getSprites()
{
	int cnt = sprites.size();

	for(int i=0; i<cnt; i++)
	{
		sprites[i].pos = particle[i].position;
	}

	return &sprites;
}


/*GameParticleSystem::GameParticleSystem(Config* _conf, ResCollection	*res)
{
	conf = _conf;

	int cnt = conf->geti("count", 1);

	string m = conf->gets("material");

	res->add(Res::config, "material/" + m);

	float size = conf->getf("size");

	sprites.resize(cnt);
	particle.resize(cnt);

	for(int i=0; i<cnt; i++)
	{
		sprites[i].material = m;
		sprites[i].width = size;
		born(i);
	}
	
}

void GameParticleSystem::process(steel::time speed)
{
	int cnt = sprites.size();

	for(int i=0; i<cnt; i++)
	{
		particle[i].position += particle[i].velocity * (float)speed;
		particle[i].lifetime -= (float)speed;
		if(particle[i].lifetime < 0)
			born(i);

	}
}

void GameParticleSystem::born(int i)
{
	coord speed = conf->getf("speed"),		life = conf->getf("lifetime");


	particle[i].position = v3(0,0,0);
	particle[i].velocity = v3(speed*(frand()-0.5f), speed*(frand()-0.5f), speed*(frand()-0.5f));
	particle[i].lifetime = frand()*life;
}

*/
