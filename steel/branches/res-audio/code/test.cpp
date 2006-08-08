/*id*********************************************************
    Unit: core
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
        Steel Engine License
    Description:
		Функция test() нужна для всевозможного тестирования кода. 
		Её можно менять как угодно, но нельзя коммитить изменения.
		Если test возвращет true, то движок продолжает загрузку, иначе
		программа завершает выполнение.

		Для отладки удобно пользоваться макросом debug()
 ************************************************************/

#include "_cpp.h"
#include "common/logger.h"

#include "res/audio/wav.h"
#include "res/res_main.h"

#include "audio/openal_engine.h"

bool test()
{

	if (!initializeOpenAL())
		return false;

	//Audio *tSnd = resAudio.add("audio/stereo");

	AL_Source *source1 = new AL_Source(resAudio.add("audio/rain"));
	AL_Source *source2 = new AL_Source(resAudio.add("audio/thunder"));

	source1->setLooped(true);
	source2->setLooped(true);
	source1->setPosition(-5.0f, 0.0f, 0.0f);
	soundUpdate(*source1);
	source2->setPosition(5.0f, 0.0f, 0.0f);
	soundUpdate(*source2);

	soundPlay(*source1);
	soundPlay(*source2);

/*
	ALuint buffer;
	ALuint source;

	ALfloat sourcePos[] = { 0.0, 0.0, 0.0 };
	ALfloat sourceVel[] = { 0.0, 0.0, 0.0 };

	ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
	ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
	ALfloat listenerOri[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };

	alGenBuffers(1, &buffer);

	alBufferData(buffer, tSnd->format, tSnd->data, tSnd->size, tSnd->frequency);


	if (tSnd->data)
		free(tSnd->data);


	alGenSources(1, &source);

	alSourcei (source, AL_BUFFER,   buffer   );
    alSourcef (source, AL_PITCH,    1.0     );
    alSourcef (source, AL_GAIN,     1.0     );
    alSourcefv(source, AL_POSITION, sourcePos);
    alSourcefv(source, AL_VELOCITY, sourceVel);
	alSourcei (source, AL_LOOPING,  tSnd->loop );

	alListenerfv(AL_POSITION,    listenerPos);
    alListenerfv(AL_VELOCITY,    listenerVel);
    alListenerfv(AL_ORIENTATION, listenerOri);


	alSourcePlay(source);
	Sleep(1000);
	alSourcePlay(source);

	//destroyOpenAL();
*/
	return true;
}
