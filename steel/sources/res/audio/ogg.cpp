/*id*********************************************************
	File: res/audio/ogg.cpp
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

#include "../../steel.h"
#include "ogg.h"
#include "../rstream.h"
#include <fstream>
#include <string>
#include "ogg/ogg.h"
#include "vorbis/codec.h"
#include "vorbis/vorbisenc.h"
#include "vorbis/vorbisfile.h"

#pragma comment(lib, "ogg.lib")
#pragma comment(lib, "vorbisfile.lib")

using namespace std;
//-----------------------------------------------------------
//    ov_callback specific functions
//-----------------------------------------------------------

size_t ReadOgg(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	istream *File = reinterpret_cast<istream*>(datasource);
	File->read((char *)ptr, size * nmemb);
	return File->gcount();
}

int SeekOgg(void *datasource, ogg_int64_t offset, int whence)
{
	istream *File = reinterpret_cast<istream*>(datasource);
	ios_base::seekdir Dir;
	File->clear();
	switch (whence) 
	{
		case SEEK_SET: Dir = ios::beg;  break;
		case SEEK_CUR: Dir = ios::cur;  break;
		case SEEK_END: Dir = ios::end;  break;
		default: return -1;
	}
	File->seekg((streamoff)offset, Dir);
	return (File->fail() ? -1 : 0);
}

long TellOgg(void *datasource)
{
	istream *File = reinterpret_cast<istream*>(datasource);
	return File->tellg();
}

int CloseOgg(void *datasource)
{
	return 0;
}

//=========================== init

bool OGG::init(const std::string& name, const std::string& dir)
{
	rstream oggFile;
	
	if (!oggFile.open(dir + "/" + name, "ogg"))
	{
		return false;
	}

	data = NULL;
	fileName = dir + "/" + name + ".ogg";


	vorbis_comment* comment;
	vorbis_info* info;

// OggVorbis specific structures
	ov_callbacks	cb;

	// Fill cb struct
	cb.close_func	= CloseOgg;
	cb.read_func	= ReadOgg;
	cb.seek_func	= SeekOgg;
	cb.tell_func	= TellOgg;
	
//	ifstream oggFile;

	// creating OggVorbis_File struct
	OggVorbis_File* ogg = new OggVorbis_File();

	// open ogg file
//	oggFile.open(fileName.c_str(), ios_base::in | ios_base::binary);

	// Generate local buffers
	if (ov_open_callbacks(oggFile, ogg, NULL, -1, cb) < 0)
	{
		// This is not ogg bitstream. Return
		return false;
	}

	// if not streamed
	unsigned long blockSize = (unsigned long) ov_pcm_total(ogg, -1) * 4;

	comment = ov_comment(ogg, -1);
	info = ov_info(ogg, -1);

	// fill Audio info
	format = (info->channels == 1) ? FORMAT_MONO16 : FORMAT_STEREO16;
	frequency = info->rate;
	

	unsigned long totalRet = 0, ret;
	int currentSection;
	char* PCM = new char[blockSize];
	while (totalRet < blockSize)
	{
		ret = ov_read(ogg, PCM + totalRet, blockSize - totalRet, 0, 2, 1, &currentSection);

		// if end of file or read limit exceeded
		if (ret == 0) break;
		else if (ret < 0) 		// Error in bitstream
		{
			//
		}
		else
		{
			totalRet += ret;
		}
	}
	if (totalRet > 0)
	{
		data = (void*) PCM;
		size = totalRet;
		//alBufferData(BufID, Buffers[BufID].Format, (void *)PCM, 
		//			 TotalRet, Buffers[BufID].Rate);
		//CheckALError();
	}
//	delete [] PCM;
	return (ret >= 0);	// fatal bug maybe... -> there was (ret > 0)


	//return true;
}