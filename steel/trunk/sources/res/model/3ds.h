/*id*******************************************************************************
	File: res/model/3ds.h
	Unit: res/model
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
        Молуль для загрузки модели формате 3ds
**************************************************************************************/

#ifndef __RES_MODEL_3DS_H__
#define __RES_MODEL_3DS_H__

#include "../../steel.h"
#include "model.h"
#include "../rstream.h"


class _3DS;

typedef int(F)(_3DS&, rstream&, int);

struct chainProcessor
{
	chainProcessor(void): tag(0) {}
	chainProcessor(unsigned short TAG, F _F): tag(TAG), f(_F) {}

	unsigned short tag;
	F *f;
};


class _3DS: public Model
{
public:
	bool init(const std::string name, const std::string dir);
};

#endif
