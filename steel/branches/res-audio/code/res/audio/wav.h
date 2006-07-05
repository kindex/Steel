#ifndef __RES_AUDIO_WAV_H
#define __RES_AUDIO_WAV_H

#include "audio.h"

class WAV : public Audio
{
    bool Open(const std::string &fileName, bool looped, bool streamed);
};

Audio * LoadWavFile(const std::string &fileName);

#endif
