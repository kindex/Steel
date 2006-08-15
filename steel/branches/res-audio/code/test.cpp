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

	//Listener *listener = new Listener();
	//updateListener(*listener);
	//listener->setOrientation(v3(0.0f, 0.0f, -1.0f), v3(0.0f, 1.0f, 0.0f));
	//updateListener(*listener);

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

	return true;
}