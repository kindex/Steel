/*id*********************************************************
	File: res/image/jpeg.h
	Unit: res/image
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		JPEG loader (using NV_UTIL lib)
 ************************************************************/

#include "../../steel.h"
#include "jpeg.h"
#include "../../common/logger.h"
#include "../rstream.h"
#include <string> 

Image* createJPEG(const std::string& filename)
{
	ResJPEG *o = new ResJPEG;
	if(o->init(filename))
	{
		o->setId(objectIdGenerator.genUid());
		return o;
	}
	else
	{
		delete o;
		return NULL;
	}
}



bool ResJPEG::init(const std::string& name)
{  
	rstream f;

	if(!f.open(name, "jpeg"))
	{
//		log_msg("warning res image tga", string("Res/Image/TGA: cannot open file ")+ name.c_str());
		return false;
	}

	return true;
}

