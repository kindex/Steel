/*id*********************************************************
	File: test.cpp
	Unit: core
	Part of: Steel engine
	(C) DiVision, 2004-2006
	Authors:
		Enyone
	License:
		Steel Engine License
	Description:
		Функция test() нужна для всевозможного тестирования кода. 
		Её можно менять как угодно, но нельзя коммитить изменения.
		Если test возвращет true, то движок продолжает загрузку, иначе
		программа завершает выполнение.

		Для отладки удобно пользоваться макросом debug()
 ************************************************************/

#include "steel.h"
#ifndef LIB_STEEL
#include "common/logger.h"

#include "res/config/config.h"
#include "res/text/text_file.h"
#include "res/config/config_collection.h"
#include "res/res_main.h"

#include <sstream>

using namespace std;

bool test()
{

/*
	Add your code for quick testing here
*/

	return true;
}

#endif
