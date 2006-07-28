#ifndef __RES_AUDIO_WAV_H
#define __RES_AUDIO_WAV_H

#include "audio.h"

class WAV : public Audio
{
public:
    bool load(const std::string &fileName/*, bool looped, bool streamed*/);
};

Audio *createWAV(const std::string &fileName);

#endif
