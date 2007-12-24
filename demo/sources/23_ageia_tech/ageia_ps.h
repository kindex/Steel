/*id*********************************************************
	Unit: 23 Ageia tech
	Part of: DiVision intro
	(C) DiVision, 2004-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Pasticle system using Ageia
 ************************************************************/

#ifndef _23_AGEIA_PS_H_
#define _23_AGEIA_PS_H_

#include "../game_free_scene.h"
#include <objects/ps/animator.h>

class NxScene;

class AgeiaPsAnimator: public ParticleAnimator
{
public:
	AgeiaPsAnimator();
	bool initParticles();
	bool InitFromConfig(Config&);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId id);
	void process(IN const ProcessInfo& info);
	bool beforeInject(IN OUT Engine&, IN const InterfaceId id);
	void afterRemove(IN OUT Engine&, IN const InterfaceId id);
	void onParticleBorn(int index);
	void onParticleDie(int index) {}

private:
	Config*					particleConf;
	NxScene*				nxScene;
};

#endif
