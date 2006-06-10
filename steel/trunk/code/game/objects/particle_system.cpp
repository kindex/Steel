/*id*********************************************************
    Unit: game/objects/sprites
    Part of: Steel engine
    Version: 1.0
    Authors:
        * def [Alexander Papyshev]
    Description:
		Класс для анимирования графической системы частиц
************************************************************/

#include "particle_system.h"
#include "../../common/utils.h"


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
	align = SpriteAlign::screen;
	initSprites();

	texCoords1D.changed = true;
	texCoords1D.id = res->genUid();
	texCoords1D.data.resize(sprite.size()*4);
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
				particle[i].born(
					curTime, 
					frameLength, 
					conf, 
					getParent()->getGlobalPosition(), getParent()->getGlobalVelocity().translation);
				limit--;
			}
	}
}

void ParticleSimple::process(steel::time curTime, steel::time frameLength)
{
	if(alive)
	{
		velocity -= velocity*speedDown*(float)frameLength;

		velocity += acc*(float)frameLength;

		position += velocity * (float)frameLength;
		if(endTime< curTime) alive = false;
// Start - 0
// End - 1
		lifeTime = (float)((curTime-startTime)/(endTime-startTime));
		if(lifeTime+EPSILON>=1) lifeTime = 1;
		size = (1-lifeTime)*startSize + lifeTime*endSize;
	}

}

void ParticleSimple::born(steel::time curTime, steel::time frameLength, Config *conf, matrix34 global, v3 globalVelocity)
{
    v3  globalsize = global * v3(1, 0, 0) - global * v3(0, 0, 0);
	float k = globalsize.getLength();

	v3	dir = conf->getv3("direction");

	dir.rotateZ(conf->getf("angle1")*G_PI * prand());
	dir.rotateAxis(conf->getf("angle2")*G_PI * prand(), v3( -dir.y, dir.x, 0));

	v3	sp = dir*(conf->getf("minspeed") + frand()*(conf->getf("maxspeed") - conf->getf("minspeed")));

	v3 initPosDelta = conf->getv3("initPosDelta");
	position = global * v3(prand()*initPosDelta.x, prand()*initPosDelta.y, prand()*initPosDelta.z);
	
	velocity = globalVelocity * conf->getf("parentSpeedK")  + sp*k;

	//4itaem i rass4itivaem vremja smerti 4astici. (eto delaetsja tolko odin raz)
	//minimal life time
	//Maximal life time
	//Random inbetween
	//TODO : horo6o bi 4itatj es4e razbros
	startTime = (float)curTime;
	endTime = (float)curTime + conf->getf("minlifetime") + frand()*(conf->getf("maxlifetime") - conf->getf("minlifetime"));

	size = conf->getf("startsize") * k;
	startSize = conf->getf("startsize") * k;
	endSize = conf->getf("endsize") * k;

	acc = conf->getv3("acc")*k;
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

TexCoords*	ParticleSystem::getTexCoords(int texNumber) 
{ 
	if(texNumber==0) // generate tex coords in 1D texture
	{
		for(unsigned int i=0; i<sprite.size(); i++)
		{
			texCoords1D.data[i*4 + 0] = 
			texCoords1D.data[i*4 + 1] = 
			texCoords1D.data[i*4 + 2] = 
			texCoords1D.data[i*4 + 3] = v2(particle[i].lifeTime, 1);
		}
		texCoords1D.changed = true;
		return &texCoords1D;
	}
	else
		return SpriteSystem::getTexCoords(texNumber);
}
