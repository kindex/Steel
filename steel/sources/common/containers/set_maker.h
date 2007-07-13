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

#ifndef _COMMON_CONTAINERS_STRING_VECTOR_H_
#define _COMMON_CONTAINERS_STRING_VECTOR_H_

#include <set>

template <typename TKey>
class SetMaker
{
public:
    SetMaker(IN const TKey& key)
    {
        dict.insert(key);
    }

    SetMaker<TKey>& operator () (IN const TKey& key)
    {
        dict.insert(key);
        return *this;
    }

    operator std::set<TKey> () const
    {
        return dict;
    }

private:
    std::set<TKey> dict;
};

#endif
