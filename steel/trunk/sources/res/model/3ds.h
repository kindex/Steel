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


class Model_3DS;

typedef int(F)(Model_3DS&, rstream&, int);

struct chainProcessor
{
	chainProcessor(void): tag(0) {}
	chainProcessor(unsigned short TAG, F _F): tag(TAG), f(_F) {}

	unsigned short tag;
	F *f;
};


class Model_3DS: public Model
{
public:
	bool init(const std::string name, const std::string dir);

	friend int chain_model_material(Model_3DS &m, rstream &f, int size);
	friend int chain_triangles(Model_3DS &m, rstream &f, int size);
	friend int chain_vertexes(Model_3DS &m, rstream &f, int size);
	friend int chain_map_coords(Model_3DS &m, rstream &f, int size);

};

#endif
