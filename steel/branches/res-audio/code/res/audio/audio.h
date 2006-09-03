/*id*********************************************************
	Unit: Audio Engine
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	File history:
		[2006.07.25 16:49:55] Kuzmi4: File Creation.
    Description:
		Звуковой движок.
 ************************************************************/

#ifndef __RES_AUDIO_H
#define __RES_AUDIO_H

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
