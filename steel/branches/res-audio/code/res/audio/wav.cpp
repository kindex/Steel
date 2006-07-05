#include "wav.h"

Audio * LoadWavFile(const std::string &fileName)
{
    SndInfo buffer;
    ALenum format;
    ALvoid * data;
    ALsizei size;
    ALsizei freq;
    ALboolean loop;
    ALuint BufID = 0;
    
    // fill buffer struct
    buffer.fileName = fileName;
    
    // check for existance of sound
    for (TBuf::iterator i = Buffers.begin(); i ! = Buffers.end(); i++)
    {
        if (i->second.fileName == fileName) 
            BufID = i->first;
    }
    
    // if such buffer not exist yet
    if (!BufID)
    {
        // generate buffers 
        alGenBuffers(1, &buffer.ID);
//        if (!CheckALError()) return false;
        
        // load WAV data
        alutLoadWAVFile((ALbyte *) fileName.data(), &format, &data, &size, &freq, &loop);
//        if (!CheckALError()) return false;

        buffer.Format = format;
        burref.Rate = freq;
        
        // fill buffer
        alBufferData(format, data, size, freq);
//        if (!CheckALError()) return false;

        Buffers[buffer.ID] = buffer;        
    }
    else
        buffer = buffer[BufID];
        
    alSourcei(itsSourceID, AL_BUFFER, buffer.ID);
    sndBuffer = buffer.ID;
    
    return true;        // MUST RETURN AUDIO RES
}

bool WAV::Open(const std::string &fileName, bool looped, bool streamed)
{
    // check file existance
    ifstream a(name.c_str());
    if (!a.is_open()) return false;
    a.close();
    
    itsFileName = name;
    
    itsLooped = Looped;
    
    // create source
    alGenSources(1, &itsSourceID);
//    if (!CheckALError()) return false;

    alSourcef(itsSourceID, AL_PITCH, pitch);
    alSourcef(itsSourceID, AL_GAIN, gain);
    alSourcefv(itsSourceID, AL_POSITION, itsPos);
    alSourcefv(itsSourceID, AL_VELOCITY, itsVel);
    alSourcei(itsSourceID, AL_LOOPING, itsLooped);

}
