/*id*********************************************************
	File: common/containters.h
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

#ifndef _COMMON_CONTAINERS_H_
#define _COMMON_CONTAINERS_H_

#include <string>
#include <map>
#include "svector.h"
#include "pvector.h"

typedef std::map<std::string, std::string> StringDict;


/*template<class T >
void append(IN OUT std::vector<T>& base, IN std::vector<T>& tail)
{
	base.insert(tail.begin(), tail.end(), base.end());
}*/

template<class T >
void append(IN OUT svector<T>& base, IN svector<T>& tail)
{
	for EACH(svector<T>, tail, it)
	{
		base.push_back(*it);
	}
}

std::string joinMap(const StringDict& container, const std::string& glue);

#endif
