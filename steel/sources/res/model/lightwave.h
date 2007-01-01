/*id*********************************************************
	File: res/model/lightwave.h
	Unit: res/model
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* Kane [J. Anton, kane@mail.berlios.de]
	License:
		Steel Engine License
	Description:
		TODO
 ************************************************************/

#ifndef __LIGHTWAVE_H__
#define __LIGHTWAVE_H__

#include "../../steel.h"
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
