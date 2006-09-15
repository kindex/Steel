/*id*********************************************************
	File: res/audio/wav.h
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

#ifndef __RES_AUDIO_WAV_H
#define __RES_AUDIO_WAV_H

#include "audio.h"


#define FORMAT_MONO8                          0x1100
#define FORMAT_MONO16                         0x1101
#define FORMAT_STEREO8                        0x1102
#define FORMAT_STEREO16                       0x1103

typedef struct				// WAV File-header
{
	unsigned char id[4];	// ALubyte
	int size;				// ALsizei
	unsigned char type[4];	// ALubyte
} WavFileHdr;

typedef struct				// WAV Fmt-header
{
	unsigned short format;			// ALushort
	unsigned short channels;		// ALushort
	unsigned int samplesPerSec;		// ALuint
	unsigned int bytesPerSec;		// ALuint
	short blockAlign;				// ALshort
	unsigned short bitsPerSample;	// ALushort
} WavFmtHdr;

typedef struct				// WAV FmtEx-header
{
	unsigned short size;				// ALushort
	unsigned short samplesPerSecond;	// ALushort
} WavFmtExHdr;

typedef struct				// Wav Smpl-header
{
	unsigned int manufacter;			// ALuint
	unsigned int product;				// ALuint
	unsigned int samplePeriod;			// ALuint
	unsigned int note;					// ALuint
	unsigned int fineTune;				// ALuint
	unsigned int SMPTEFormat;			// ALuint
	unsigned int SMPTEOffset;			// ALuint
	unsigned int loops;				// ALuint
	unsigned int samplerData;			// ALuint
	struct
	{
		unsigned int identifier;		// ALuint
		unsigned int type;				// ALuint
		unsigned int start;			// ALuint
		unsigned int end;				// ALuint
		unsigned int fraction;			// ALuint
		unsigned int count;			// ALuint
	} Loop[1];
} WavSmplHdr;

typedef struct
{
	unsigned char id[4];				// ALubyte
	unsigned int size;					// ALuint
} WavChunkHdr;



class WAV : public Audio
{
public:
	bool init(const std::string name, const std::string dir);
};

#endif
