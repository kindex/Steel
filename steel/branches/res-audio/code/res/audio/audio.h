#ifndef __RES_AUDIO_H
#define __RES_AUDIO_H

#include "../res.h"

#include "../../audio/openal_engine.h"

/*
#include <string>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <map>
*/

//using namespace std;

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
    Audio();
    ~Audio();
    
    ALfloat itsVelocity[3];
    ALfloat itsPosition[3];
    bool isLooped;
    std::string itsFileName;
    
    float gain;
    float pitch;
    
    unsigned int sndBuffer;

	//bool load(const std::string &fileName);
    
//private:    
    ALuint itsSourceID;
//    bool LoadWavFile(const std::string &fileName);
};

#endif
