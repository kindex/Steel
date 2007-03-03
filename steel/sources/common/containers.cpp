/*id*********************************************************
	File: common/containters.cpp
	Unit: logger
	Part of: Steel engine
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Полезные типы и функции
 ************************************************************/

#include "containers.h"

std::string joinMap(const StringDict& container, const std::string& glue)
{
	StringDict::const_iterator it = container.begin();
	if (it == container.end())
	{
		return std::string();
	}
	std::string result = it->first + "=" + it->second;
	it++;
	while(it != container.end())
	{
		result += glue;
		result += it->first + "=" + it->second;
		it++;
	}
	return result;
}
