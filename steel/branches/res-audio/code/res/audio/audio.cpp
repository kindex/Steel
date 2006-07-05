#include "audio.h"

Audio::Audio()
{
    for (int i = 0; i < 3; i++)
    {
        itsPos[i] = itsVel[i] = 0.0f;
    }
    itsFileName = " ";
    gain = 1.0f;
    pitch = 1.0f;
}

Audio::~Audio()
{
}
