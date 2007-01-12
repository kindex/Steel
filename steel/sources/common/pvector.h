/*id**********************************************************
	File: common/pvector.h
	Unit: vector
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		plain vector: Аналог std::vector для простых типов (int, pointer,
		структуры без конструктора). Нет дефолтовой инициализации.
 ************************************************************/
#ifndef __COMMON__PVECTOR_H__
#define __COMMON__PVECTOR_H__

#if STEEL_VECTOR == 1
	#include <stdlib.h>
#else
	#include <vector>
#endif

#include "../steel.h"

namespace steel
{

/*	template <typename T>
	class vector: public std::vector<T>
	{
	public:
		vector(size_t size): std::vector<T>::vector(size) {}
        vector(): std::vector<T>::vector() {}
	};
*/	


#if STEEL_VECTOR != 1

	template <typename T>
	class pvector: public std::vector<T>
	{
	public:
		pvector(size_t size): std::vector<T>::vector(size) {}
		pvector(): std::vector<T>::vector() {}
	};

#else

// define new steel::vector with std::vector interface and fast realization for simple classes (without constructors)

template <typename T>
class pvector
{
	size_t _count;// Количество используемых эелементов
	T *data;  // динамический массив для хранения данных
	size_t _size; // Размер массива

public:
	typedef T* iterator;
	typedef iterator const_iterator;
	typedef T value_type;

	pvector() 
	{ 
		_size = _count = 0; 
		data = NULL; 
	}

	pvector(const pvector &original)
	{ 
		_size = original._size;
		_count = original._count;
		data = (T*)malloc(_size * sizeof(T));
		memcpy(data, original.data, _count* sizeof(T));
	}

	pvector& operator=(const pvector& original)
	{ 
		_size = original._size;
		_count = original._count;
		data = (T*)malloc(_size * sizeof(T));
		memcpy(data, original.data, _count* sizeof(T));
		return *this;
	}

	pvector(size_t initSize) 
	{ 
		_size = _count = initSize; 
		data = (T*)malloc(_size*sizeof(T)); 
	}

	~pvector() 
	{
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

		if(oldCount*2 > _count)
		{
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

	T&operator[](size_t i)	
	{		
		return data[i];	
	}
	
	const T&operator[](size_t i) const	
	{		
		return data[i];	
	}

	T& at(size_t i) 
	{		
		return data[i];	
	}

	const T& at(size_t i) const	
	{		
		return data[i];	
	}

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

	T &back()
	{		
		return data[_count-1];
	}
	
	T &front()
	{		
		return data[0];	
	}

	const T &back()	const 
	{		
		return data[_count-1];	
	}
	
	const T &front()	const 
	{		
		return data[0];	
	}

	void erase(const iterator it)
	{
		int move = ((data + _count -1) - it)*sizeof(T);
		if(move > 0)
		{
			memcpy(it + 1, it, move);
		}
		_count--;
	}

};
#endif // STEEL_VECTOR

}

using steel::pvector;

#endif // __COMMON__PVECTOR_H__

