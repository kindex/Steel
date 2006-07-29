#ifndef __RES_AUDIO_WAV_H
#define __RES_AUDIO_WAV_H

#include "audio.h"

class WAV : public Audio
{
public:
    bool init(const std::string name, const std::string dir);
};

#endif
