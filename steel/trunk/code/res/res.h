/*id*******************************************************************************
    Unit: Res [Resources]
    Part of: Steel engine
    (C) DiVision, 2004-2006
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    License:
        Steel Engine License
    Description:
        Молуль для хранения, загрузки игровых ресурсов и контроля над ними. 
	Parts:
		res.cpp
		В поддиректория хранятся классы для загрузки и хранения всех типов ресурсов.
**************************************************************************************/

/*-----------------------------------------------------------------------

    Full Description:
        Молуль для хранения, загрузки и контроля над игровыми ресурсами, 
		такими как: image, model, audio.
		Главный класс ResCollection получает запросы на загрузку ресурсов, 
		обрабатывает их, исключает дублирование ресурсов.
		Потомки класса Res используются для кранения  и рагрузки ресурсов по типам.
		Res::res_kind определяет типы ресурсов.
		Пример использования:

Res* createBMP(string filename) {return new BMP(filename); }

	res.registerClass(createBMP,	Res::image);

	res.add(Res::image, "box");

	res["Image"]->bitmap ... 

-----------------------------------------------------------------------*/


#ifndef __RES_H
#define __RES_H

#include <map>
#include <string>
#include <fstream>
#include <stack>

#include "../interface.h"

#include "../common/types.h"
#include "../steel.h"
#include "../_cpp.h"


/*
Resourse input file stream
Все рагрузки ресурсов из файловой системы должны использовать этот класс для чтения из файлов
От него наследуются загрузки из файла, архива или скачивания из инета.
*/
class rstream: public std::ifstream
{
public:
	rstream() {}
	rstream(std::string s, std::string ext = "", ios_base::openmode _Mode = std::ios::binary) 
	{ 
		open(s, ext, _Mode);
	}

	bool open(std::string s, std::string ext = "", ios_base::openmode _Mode = std::ios::binary);

	void read(void *dest, int size);
	void skip(int n);// skip n byten in input stream
};


class ResStack
{
protected:
	int level;
	std::stack<std::string> stack;

public:
	ResStack(): level(0) {}

	bool	push(std::string directory);
	bool	pop(void);
	int		getLevel(void);
	std::string top(void);
};

extern ResStack resStack;

/*
Класс - храниель и рагрузчик одного ресурса.
Хранение - стандартизовано для каждого ресурса. Загруска - для каждого типа может быть несколько классов для загрузки.

От этого класса наследуются классы для хранения ресурсов для каждого типа (image, model).
В них реализуется unload, который уничтожает структуры, созданные при загрузке.
От них наследуются загрузчики, которые переопределяют метод init.
init загружает ресурс или генерирует по строковому идентификатору.

Например, загружая cubeMap надо загрузить 6 текстур, а только потом собрать из них одну. 
Для загрузки каждой из этих текстур используется класс image, который может грузить из любого
формата (bmp, jpg, png).
*/
class Res//: public virtual BufferedElement
{
public:
// количестко типов ресурсов
#define RES_KIND_COUNT 6
// типы ресурсов (типы хранения)
	typedef enum 
	{
			none,
			image, // class Image: 2D, 3D, Cubemap (1x6 maps in one)
			model, // class Model
			config, // class Config
			script,
			material

	}	res_kind;
// Структура для идентификации ресурса (тип, строка)
	struct ResLocator
	{
		res_kind kind;
		std::string name;

		ResLocator(res_kind akind, std::string aname): kind(akind), name(aname) {}
	};

	typedef
		steel::vector<ResLocator>
		ResLocatorArray;

//	virtual bool init(const std::string name, const std::string dir) = 0;
	virtual bool unload() = 0;

	uid id;
	uid	getId() { return id; }
	void setId(uid _id) { id = _id; }
};


/*
Коллекция ресурсов
Хранит множестко ресурсов различных типов. 
Ресурсы идентифициются строкой, которая используется при загрузке (инициализации ресурса).
У каждого ресурса должен быть уникальный строковой идентификатор, который не включает в себя расширение файла.
При вызове метода add, коллекция пытается загрузить ресурс всеми возможными способами для загрузки ресурса этого типа.
Например, для загрузки ресурса типа "image" вызовутся загрузчики BMP, JPG, PNG. Если хоть один из них сможет загрузить 
ресурс - рагзузиться, иначе - нет.
*/
template<class T>
class ResCollection
{
protected:
	// Идентификатор коллекции (model, image, ...)
	std::string id;
	bool lastCached;
// Массив с классами, в которых хранятся ресурсы
	steel::vector<T*> data;
// Map: resource name->index
// По имени возврашает индекс в массивах data и resType
	std::map<const std::string,int> index; 
	steel::vector<std::string> names;


	// тип: функция для геренирования копии класса, унаследованного от Res
	typedef T*(funcCreateResClass)(const std::string filename, const std::string baseName); 

	// копия класса. Нужна для ассоциирования класса с типом ресурса.
	struct ClassCopy
	{
		funcCreateResClass* func;

		ClassCopy(): func(NULL) {}
		ClassCopy(funcCreateResClass* _func){ func = _func;}

	};

	
// Массивы классов для рагрузки ресурсов каждого типа
	steel::vector<ClassCopy> classes;
// Количество рагруженных ресурсов (в начале 0)
	int freeindex;
public:
	ResCollection(): freeindex(0) {}
	void setId(std::string _id) { id = _id; }

	// Вернуть ресурс по номеру
	inline T* operator[](const int n) const { return data[n]; }

	// Проверить, существует ли ресурс с указанным именем
	inline bool find(const std::string& name) {return index.find(name) != index.end(); } 

	// Найти номер ресурса по имени
    inline int getIndex(const std::string name);
    
	// Вернуть ресурс по имени
	inline T* operator[] (const std::string& name);

	// Вернуть ресурс по имени и типу
	inline T* get(const std::string& name) { return operator[](name); }

/*
	Добавить ресурс по имени и типу
	name - идентификатор ресурса. Обычно это имя файла без расширения, 
	но может быть ипрограммно генерируемой тестурой.
	Пробуем загрузить всеми доступными загрузчиками по порядку.
*/
	T* addForce(std::string name, bool pop = true);

	// Добавить ресурс по имени и типу, если ресурса еще нет в коллекции
	T* add(const std::string name, bool pop = true);

	// Добавить массив ресурсов
	bool add(const Res::ResLocatorArray &names);

	// Ассоциировать тип ресурса и загрузчик.
	// _func - функция для создания класса для загрузки (унаследоманного от Res)
	void registerClass(funcCreateResClass *_func)	{		classes.push_back(ClassCopy(_func));	}

	// Вернуть копию класса
	T* createClass(ClassCopy *aclass, std::string name, std::string baseName)	{		return aclass->func(name, baseName);	}

	void pop() { if(!lastCached) resStack.pop(); }
};

std::string getFullPath(std::string filename, std::string directory);

template<class T>
bool ResCollection<T>::add(const Res::ResLocatorArray &names)
{
	for(Res::ResLocatorArray::iterator it = names.begin();	it != names.end();	it++)
		{
			if(!find(it->name))
			{
				if(!addForce(it->kind, it->name)) return false;
			}
		}
	return true;
}

template<class T>
T* ResCollection<T>::add(const std::string name, bool pop)
{
	if(getIndex(name) < 0)
	{
		lastCached = false;
		return addForce(name, pop);
	}
	else
	{
		lastCached = true;
		return get(name);
	}
}

template<class T>
T* ResCollection<T>::addForce(std::string name, bool pop)
{
	std::string baseDirectory = resStack.top();
	name = getFullPath(name, baseDirectory);

	splitPath(name, baseDirectory, name);
	resStack.push(baseDirectory);

	log_msg("res " + id, "Loading " + baseDirectory + "/" + name);

    int s = classes.size();
	for(int i = 0; i < s; i++)
	{
		T *obj = createClass(&classes[i], name, baseDirectory); // + baseDirectory
		if(obj == NULL) continue;
		
		index[name] = freeindex;
		data.resize(freeindex+1);
		names.resize(freeindex+1);
	    
		std::string fullResName = baseDirectory + "/" + name;
		index[fullResName] = freeindex;
		names[freeindex] = fullResName;

		data[freeindex] = obj;
	
		log_msg("res " + id, "OK " + baseDirectory + "/" + name);
		if(pop) resStack.pop();

		freeindex++;
		return data[freeindex-1];
	}
	log_msg("res " + id, "Failed " + baseDirectory + "/" + name);
	if(pop) resStack.pop();

	return NULL;
}

// Вернуть ресурс по имени
template<class T>
inline T* ResCollection<T>::operator[] (const std::string& name) 
{
	int i = getIndex(name);
	if(i<0)
		return NULL;
	else
		return data[i];
}

template<class T>
inline int ResCollection<T>::getIndex(const std::string name)   
{
	std::string realname = getFullPath(name, resStack.top());
	if(index.find(realname) != index.end())
		return index[realname];	
	else 
		return -1;
}  /*If exist - return*, esle 0 */


#endif
