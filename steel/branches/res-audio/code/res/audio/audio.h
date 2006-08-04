#ifndef __RES_AUDIO_H
#define __RES_AUDIO_H

#include "../res.h"

//#include "../../audio/openal_engine.h"

/*
#include <string>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <map>
*/

using namespace std;

/*
typedef struct
{
    unsigned int ID;
	string fileName;
    unsigned int Rate;
    unsigned int Format;
} SndInfo;

typedef map<ALuint, SndInfo> TBuf;

extern TBuf buffers;
*/
//ALCdevice *pDevice;
//ALCcontext *pContext;




class Audio : public Res
{
public:
	void *data;
	char *fileName;
	int format;
	int size;
	int frequency;
	bool loop;
};

#endif
