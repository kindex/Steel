#include "wav.h"


Audio *createWAV(const std::string &fileName)
{
	WAV *o = new WAV;
	if (o->load(fileName))
	{
		return o;
	}
	else
		return 0;	// NULL
}


bool WAV::load(const std::string &fileName)
{
    SndInfo buffer;
    ALenum format;
    ALvoid * data;
    ALsizei size;
    ALsizei freq;
    ALboolean loop;
    ALuint BufID = 0;

	// check file existance
	ifstream a(fileName.c_str());
    if (!a.is_open()) 
		return false;
    a.close();

		// maybe here extractExtension() function

/*
	string Ext;
	if (Ext != "WAV")
	{
		return false;

	}
*/
    
    itsFileName = fileName;

    // fill buffer struct
    buffer.fileName = fileName;
    
    // check for existance of sound
    for (TBuf::iterator i = buffers.begin(); i != buffers.end(); i++)
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
        buffer.Rate = freq;
        
        // fill buffer
		alBufferData(buffer.ID, format, data, size, freq);
//        if (!CheckALError()) return false;

		// unload wav file
		alutUnloadWAV(format, data, size, freq);
//        if (!CheckALError()) return false;

        buffers[buffer.ID] = buffer;        
    }
    else
	        buffer = buffers[BufID];
    
	// create source
    alGenSources(1, &itsSourceID);
//    if (!CheckALError()) return false;

    alSourcef(itsSourceID, AL_PITCH, pitch);
    alSourcef(itsSourceID, AL_GAIN, gain);
    alSourcefv(itsSourceID, AL_POSITION, itsPosition);
    alSourcefv(itsSourceID, AL_VELOCITY, itsVelocity);
    alSourcei(itsSourceID, AL_LOOPING, isLooped);

    alSourcei(itsSourceID, AL_BUFFER, buffer.ID);
    sndBuffer = buffer.ID;
    
    return true;
}
