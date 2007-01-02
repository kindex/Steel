/*id**********************************************************
	File: common/svector.h
	Unit: vector
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
		Аналог std::vector
 ************************************************************/
#ifndef __STEEL_VECTOR_H__
#define __STEEL_VECTOR_H__

/*public ref class DebuggerVisualizerAttribute sealed : public Attribute
{
};*/

#include <stdlib.h>
#include <vector>

#include "../steel.h"

namespace steel
{

	// rename std::vector to steel::vector
	
	template <typename T>
	class vector: public std::vector<T>
	{
	public:
		vector(size_t size): std::vector<T>::vector(size) {}
        vector(): std::vector<T>::vector() {}
	};
	


#ifndef STEEL_VECTOR

	template <typename T>
	class svector: public std::vector<T>
	{
	public:
		svector(size_t size): std::vector<T>::vector(size) {}
		svector(): std::vector<T>::vector() {}
	};

#else

// define new steel::vector with std::vector interface and fast realization for simple classes (without constructors)

template <typename T>
class svector
{
	size_t _count;// Количество используемых эелементов
	T *data;  // динамический массив для хранения данных
	size_t _size; // Размер массива

public:
	typedef T* iterator;
	typedef iterator const_iterator;

	svector() 
	{ 
		_size = _count = 0; 
		data = NULL; 
	}

	svector(const svector &original)
	{ 
		_size = original._size;
		_count = original._count;
		data = (T*)malloc(_size * sizeof(T));
//		memcpy(data, original.data, _count* sizeof(T));
		for (size_t i = 0; i < _count; i++)
		{
			new (&data[i]) T(original.data[i]);
		}
	}

	svector& operator=(const svector& original)
	{ 
		_size = original._size;
		_count = original._count;
		data = (T*)malloc(_size * sizeof(T));
//		memcpy(data, original.data, _count* sizeof(T));
		for (size_t i = 0; i < _count; i++)
		{
			new (&data[i]) T(original.data[i]);
		}
		return *this;
	}

	svector(size_t initSize) 
	{ 
		_size = _count = initSize; 
		data = (T*)malloc(_size*sizeof(T)); 
	}

	~svector() 
	{
		while (_count > 0)
		{
			_count--;
			data[_count].~T();
		}
		free(data);
	}

	void resize(size_t newCount)
	{
		size_t oldCount = _count;
		if(newCount <= _size)
		{
			_count = newCount;
		}
		else // need resize
		{
			size_t newSize = 2*_size;
			if(newSize < newCount)
				newSize = newCount;
			_size = newSize;
			_count = newCount;
			data = (T*)realloc(data, _size*sizeof(T));
		}

		if(oldCount < _count)
		{
//			memset((char*)data + oldCount*sizeof(T), 0, (_count - oldCount)*sizeof(T));
			for(size_t i = oldCount; i < _count; i++)
			{
				new (&data[i]) T;
			}
		}
		if(oldCount > _count)
		{
			for(size_t i = _count; i < oldCount; i++)
			{
				data[i].~T();
			}
			pack();
		}

	}
	
	void clear()
	{
		resize(0);
		pack();
	}

	size_t size() const { return _count; }
	size_t capacity() const { return _size; }
	bool empty() const { return _count==0; }

	T&operator[](size_t i)	{		return data[i];	}
	const T&operator[](size_t i) const	{		return data[i];	}
	T& at(size_t i) {		return data[i];	}
	const T& at(size_t i) const	{		return data[i];	}

	void pack()
	{
		if(_count < _size)
		{
			_size = _count;
			data = (T*)realloc(data, _size*sizeof(T));
		}
	}

	void push_back(const T newItem)
	{
		resize(_count+1);
		data[_count-1] = newItem;
	}
	
	void pop_back()
	{
		resize(_count - 1);
	}

	iterator begin() const
	{ 
		if(_count == 0)
			return NULL;
		else
			return data; 
	}

	iterator end() const
	{ 
		if(_count == 0)
			return NULL;
		else
			return data + _count;
	}

	T &back()	{		return data[_count-1];	}
	T &front()	{		return data[0];	}
	const T &back()	const {		return data[_count-1];	}
	const T &front()	const {		return data[0];	}
	void erase(const iterator it)
	{
		int move = ((data + _count -1) - it)*sizeof(T);
		if(move > 0)
		{
			memcpy(it + 1, it, move);
		}
		_count--;
	}

	iterator find(const T &value)
	{
		iterator it;
		for(it = begin(); it != end(); it++)
			if( *it == value)
				break;
		return it;
	}
};
#endif // STEEL_VECTOR


}

#endif // __STEEL_VECTOR_H__

using steel::svector;
