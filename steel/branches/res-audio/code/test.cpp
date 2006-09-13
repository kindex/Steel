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
//#include "res/res_main.h"

#include "audio/openal_engine.h"

bool test()
{
	if (!initializeOpenAL())
		return false;
/*
	Listener *listener = new Listener();
	updateListener(*listener);
	listener->setOrientation(v3(0.0f, 0.0f, -1.0f), v3(0.0f, 1.0f, 0.0f));
	updateListener(*listener);
*/
	//Audio *tSnd = resAudio.add("audio/stereo");

//	Source *source1 = new Source(resAudio.add("audio/rain"));
//	Source *source2 = new Source(resAudio.add("audio/thunder"));

	//alSourcef(source1->source, AL_ROLLOFF_FACTOR, 0.0f);
	//alSourcef(source2->source, AL_ROLLOFF_FACTOR, 0.0f);
/**
	source1->setLooped(true);
	source2->setLooped(true);
	source1->setPosition(0.0f, 0.0f, 0.0f);
	soundUpdate(*source1);
	source2->setPosition(-10.0f, 0.0f, 0.0f);
	soundUpdate(*source2);

	soundPlay(*source1);
	soundPlay(*source2);
**/

	AudioEngine *audio = new AudioEngine();
	audio->setConfig();
	audio->loadSources();

	//setListenerEnvironment(EAX_ENVIRONMENT_SEWERPIPE);

	audio->sources[0].setPosition(0.0f, 0.0f, 0.0f);
	soundUpdate(audio->sources[0]);
	soundPlay(audio->sources[0]);

	audio->sources[1].setPosition(-10.0f, 0.0f, 0.0f);
	soundUpdate(audio->sources[1]);
	soundPlay(audio->sources[1]);

	return true;
}