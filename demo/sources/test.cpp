/*id*********************************************************
	Unit: core
	Part of: DiVision intro
	(C) DiVision, 2006
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

#include <steel.h>
#include <common/logger.h>

#include <objects/combiner/combiner.h>
#include <objects/combiner/audio/simple_sound.h>


bool test()
{
	return true;
}


bool test(AudioEngine* audio)
{
	return true;
}
