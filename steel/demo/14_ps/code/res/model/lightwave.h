/***************************************************************************
 *            lightwave.h
 *
 *  Sat Mar  4 09:24:24 2006
 *  Copyright  2006  J. Anton
 *  kane@mail.berlios.de
 ****************************************************************************/

#ifndef __LIGHTWAVE_H__
#define __LIGHTWAVE_H__

#include "bones.h"
#include "model.h"

class LightWaveAnimation: public Bones
{
	virtual bool init(std::string& name);
	virtual bool unload() { return true; }
};

class LightWave: public Model
{
	virtual bool init(std::string& name);
	virtual bool unload() { return true; }
};

#endif
