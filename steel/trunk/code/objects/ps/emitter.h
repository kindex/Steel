/*id*********************************************************
    Unit: particle_system
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
		* Def [Alexander Papyshev, 9000gl@gmail.com, aleksandrs.papisevs@gmail.com]
    License:
        Steel Engine License
    Description:
		Steel ������ ����������� ������. 
	������������ �������� ������ ������.
************************************************************/
#ifndef __PARICLE_SYSTEM__EMITTER_H
#define __PARICLE_SYSTEM__EMITTER_H

#include "particle_system.h"

class SimpleEmitter: public ParticleEmitter
{
public:
	void born(Particle &particle); // ������� �������
	void ProcessPhysic(steel::time curTime, steel::time frameLength, ModificationTime modificationTime);

};

#endif
