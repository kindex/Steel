/*id*********************************************************
	File: res/audio/off.h
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

#ifndef __RES_AUDIO_OGG_H
#define __RES_AUDIO_OGG_H

#include "../../steel.h"
#include "audio.h"

#include "ogg/ogg.h"
#include "vorbis/codec.h"
#include "vorbis/vorbisenc.h"
#include "vorbis/vorbisfile.h"

#pragma comment(lib, "ogg.lib")
#pragma comment(lib, "vorbisfile.lib")

class OGG : public Audio
{
public:
	bool init(const std::string name, const std::string dir);
};

#endif
