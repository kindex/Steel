/*id*********************************************************
	File: common/containters/string_dict.h
	Unit: containters
	Part of: Steel engine
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		String Dict
 ************************************************************/

#ifndef _COMMON_CONTAINERS_STRING_DICT_H_
#define _COMMON_CONTAINERS_STRING_DICT_H_

#include <string>
#include <map>
#include "../svector.h"

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

// find in vector, svector, pvector
template<typename C>
typename C::const_iterator find(const C& container, typename const C::value_type& value)
{
	for EACH_CONST(C, container, it)
	{
		if (*it == value)
		{
			return it;
		}
	}
	return container.end();
}

#endif
