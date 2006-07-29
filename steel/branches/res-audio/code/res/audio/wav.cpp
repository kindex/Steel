#include "wav.h"
#include "../rstream.h"


bool WAV::init(const std::string name, const std::string dir)
{
    SndInfo buffer;
    ALenum format;
    ALvoid * data;
    ALsizei size;
    ALsizei freq;
    ALboolean loop;
    ALuint BufID = 0;

	// check file existance
	rstream f;
	
	if(!f.open(dir + "/" + name, "wav"))
		return false;

		// maybe here extractExtension() function

/*
	string Ext;
	if (Ext != "WAV")
	{
		return false;

	}
*/

	filebuf *pbuf;
	long sizel;
	char *buffer_;

	pbuf = f.rdbuf();
	// get file size using buffer's members
	sizel = pbuf->pubseekoff (0,ios::end,ios::in);
	pbuf->pubseekpos (0,ios::in);
	// allocate memory to contain file data
	buffer_ = new char[sizel];
	// get file data  
	pbuf->sgetn (buffer_, sizel);
	f.close();
 
    
   
    // check for existance of sound
/*    for (TBuf::iterator i = buffers.begin(); i != buffers.end(); i++)
    {
        if (i->second.fileName == fileName) 
            BufID = i->first;
    }*/
    
    // if such buffer not exist yet
    if (!BufID)
    {
        // generate buffers 
        alGenBuffers(1, &buffer.ID);
//        if (!CheckALError()) return false;
        
		size = sizel;

        // load WAV data
        alutLoadWAVMemory((ALbyte *)buffer_, &format, &data, &size, &freq, &loop);
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
    
	delete buffer_;

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
