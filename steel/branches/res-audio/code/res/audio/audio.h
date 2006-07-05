#ifndef __RES_AUDIO_H
#define __RES_AUDIO_H

#include "../res.h"

typedef struct
{
    unsigned int ID;
    string fileName;
    unsigned int Rate;
    unsigned int Format;
} SndInfo;

class Audio : public Res
{
public:    
    Audio();
    ~Audio();
    
    ALfloat itsVel[3];
    ALfloat itsPos[3];
    bool itsLooped;
    std::string itsFileName;
    
    float gain;
    float pitch;
    
    unsigned int sndBuffer;
    
private:    
    ALuint itsSourceID;
//    bool LoadWavFile(const std::string &fileName);
};

#endif
