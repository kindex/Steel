#ifndef _COMMON_CONTAINERS_H_
#define _COMMON_CONTAINERS_H_

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

#endif
