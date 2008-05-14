/*id*********************************************************
	File: common/containters/string_vector.h
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

template<typename K, typename S>
bool present(const K& key, const S& set)
{
    return set.find(key) != set.end();
}

template<typename K, typename S>
void erase(const K& key, S& set)
{
    set.erase(set.find(key));
}
