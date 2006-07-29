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

	Audio *tSnd = resAudio.add("audio/stereo");

	alSourcePlay(tSnd->itsSourceID);

	//destroyOpenAL();
	return true;
}
