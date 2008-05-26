/*id*********************************************************
	File: common/cast.h
	Unit: containters
	Part of: Steel engine
	(C) DiVision, 2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		StringVector
 ************************************************************/
#pragma once
#include <sstream>

template <typename To, typename From>
inline To cast(IN const From from)
{
    std::stringstream ss;
    To to;
    ss << from;
    ss >> to;
    return to;
}
