#ifndef VECTOR_H
#define VECTOR_H

/*public ref class DebuggerVisualizerAttribute sealed : public Attribute
{
};*/


#include <stdlib.h>

namespace steel
{

template <typename T>
class vector
{
	int _count;// Количество используемых эелементов
	T *data;  // динамический массив для хранения данных
	int _size; // Размер массива

public:
	vector() { _size = _count = 0; data = NULL; }
	void resize(int newCount)
	{
		if(newCount <= _size)
		{
			_count = newCount;
			if(_count*4 < _size)
				pack();
		}
		else // need resize
		{
			int newSize = 2*_size;
			if(newSize < newCount)
				newSize = newCount;
			_size = newSize;
			_count = newCount;
			data = (T*)realloc(data, _size*sizeof(T));
		}
	}
	
	void clear()
	{
		_size = 0;
		_count = 0;
		free(data); data = NULL;
	}

	int size() { return _count; }
	int capacity() { return _size; }
	bool empty() { return _count==0; }

	T&operator[](int i)
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
	
	T pop_back()
	{
		T result = data[_count-1];
		resize(_count - 1);
		return result;
	}

};

}

#endif
