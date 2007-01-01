/*id*********************************************************
	File: res/audio/audio.h
	Unit: res audio
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		TODO
 ************************************************************/

#ifndef __RES_AUDIO_H
#define __RES_AUDIO_H

#include "../../steel.h"
#include "../res.h"


using namespace std;



class Audio : public Res
{
public:
	void *data;
	std::string fileName;
	//char *fileName;
	int format;
	int size;
	int frequency;
	bool loop;
};

#endif
