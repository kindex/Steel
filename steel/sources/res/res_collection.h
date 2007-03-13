/*id*******************************************************************************
	File: res/res_collection.h
	Unit: res
	Part of: Steel engine
	(C) DiVision, 2003-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Молуль для хранения, загрузки игровых ресурсов и контроля над ними. 
	Parts:
		res.cpp
		В поддиректориях хранятся классы для загрузки и хранения всех типов ресурсов.
**************************************************************************************/

#ifndef RES_COLLECTION_H
#define RES_COLLECTION_H

#include "../steel.h"

#include <map>
#include <set>
#include <string>

#include "../common/types.h"
#include "../common/utils.h"
#include "../common/logger.h"
#include "../engine/id_generator.h"
#include "res.h"


// Собирает полное имя файла относительно директории. Если имя файла начинается с /, то имя файла считается уже полным
std::string getFullPath(const std::string& filename, const std::string& directory);

// Стек текущих директорий. Используется только коллекцией ресурсов.
class ResStack
{
public:
	ResStack(): level(0), dirs(0) {}

	bool	push(std::string directory);
	bool	pushFullPath(std::string path);

	bool	pop();
	int		getLevel();
	std::string top();
	std::string getFullName(const std::string& name);

protected:
	int level;
	svector<std::string> dirs;

	template<class T>
	friend class ResCollection;
};
extern ResStack resStack;

/*
Коллекция ресурсов
Хранит множестко ресурсов различных типов. 
Ресурсы идентифициются строкой, которая используется при загрузке (инициализации ресурса).
У каждого ресурса должен быть уникальный строковой идентификатор, который не включает в себя расширение файла.
При вызове метода add, коллекция пытается загрузить ресурс всеми возможными способами для загрузки ресурса этого типа.
Например, для загрузки ресурса типа "image" вызовутся загрузчики BMP, JPG, PNG. Если хоть один из них сможет загрузить 
ресурс - то он рагзузится, иначе - нет.
*/
template<class T>
class ResCollection
{
public:
	ResCollection() {}

	inline T* operator[] (const std::string& name);// Вернуть ресурс по полному имени
	inline T* get(const std::string& name) { return operator[](name); }  // Вернуть ресурс по полному имени

	// Добавить ресурс по имени и типу, если ресурса еще нет в коллекции
	T* add(const std::string& name, bool pop = true);

	// Удалить ресурс
	bool remove(const std::string& name);
	bool remove(T* object);

	// У процедуры add есть второй параметр типа bool. Если он равняется false, то после загрузки ресурса текущая директория не восстанавливается и надо это делать вручную с помощью вызова pop.
	void pop() { resStack.pop(); }
	// тип: функция для геренирования копии класса, унаследованного от Res
	typedef T*(funcCreateResClass)(const std::string& filename, const std::string& baseName); 
	// Добавляет функцию для создания класса, котоыре умеет загружать ресурс
	void registerResLoader(funcCreateResClass* _func)	{		classes.push_back(_func);	}
	void setId(const std::string& _id) { id = _id; } // устанавливает идентификатор коллекии
	void clearFailedResourcesCache() { failedResourcesCache.clear(); }

protected:

// Массивы классов для рагрузки ресурсов каждого типа
	pvector<funcCreateResClass*> classes;

	// *********** Functions ***************
	bool removeRaw(int index); // удаляет ссылку на ресурс. Если количество ссылок = 0, то ресурс удалается физически из коллекции
	virtual bool purge(int delIndex); // физически удаляет ресурс из коллекции

	// Найти номер ресурса по полному имени
    inline int getIndex(const std::string& name);
	// Проверить, существует ли ресурс с указанным полным именем
	inline bool find(const std::string& name) {return index.find(name) != index.end(); } 

	virtual T* addForce(const std::string& name, bool pop = true);


	std::string id; // строковой идентификатор коллекции (image, model)
	std::map<const std::string,int> index; // отображение полных имён ресурсов на индекс в массиве data. Для увеличения скорости поиска по имени.
	std::map<T*,int> resIndex; // отображение указателя ресурса на индекс в массиве data. Для увеличения скорости удаления ресурса по ссылке на него.

	struct ResStorage
	{
		T*		object;		// ссылка на ресурс
		uid		id;			// уникальный идентификатор ресурса
		int		links;		// количество ссылок на этот ресурс. При добавлении дублируемого ресурса счётчик увеличивается на 1. При удалении -  уменьшается на 1. Если количество ссылок становится равным 1, то ресурс удаляется из коллекции. (аналогично Linux ext2fs)
		std::string name;	// полное имя ресурса
	};
	svector<ResStorage> data; // массив хранимых ресурсов и дополнительная информация к каждому ресурсу.
	std::set<std::string> failedResourcesCache;
};



template<class T>
T* ResCollection<T>::add(const std::string& name, bool pop)
{
	if (name.empty() || name == "/")
	{
		return NULL;
	}

	std::string name2 = resStack.getFullName(name);
	int index = getIndex(name2);

	if (index < 0)
	{
		return addForce(name2, pop);
	}
	else
	{
		data[index].links++;

		if(!pop)
		{
			resStack.pushFullPath(name2);
		}

		return data[index].object;
	}
}

template<class T>
bool ResCollection<T>::remove(T* object)
{
#if STEEL_COMPILER == COMPILER_VS8
    std::map<T*,int>::const_iterator it = resIndex.find(object);
     
	if(it == resIndex.end())
	{
		return false;
	}
	else
	{
		return removeRaw(it->second);
	}
#else
// TODO
	if(resIndex.find(object) == resIndex.end())
		return false;
	else 
		return removeRaw(resIndex.find(object)->second);
#endif
}


template<class T>
bool ResCollection<T>::remove(const std::string& name)
{
	int index = getIndex(name);
	if (index < 0)
	{
		return false;
	}
	else
	{
		return removeRaw(index);
	}
}

template<class T>
bool ResCollection<T>::removeRaw(int index)
{
	if (data[index].links > 0)
	{
		data[index].links--;
		if(data[index].links == 0)
		purge(index);
		return true;
	}
	else
	{
		return false;// уже удалён
	}
}

template<class T>
bool ResCollection<T>::purge(int delIndex)
{
	std::string name;
	name = data[delIndex].name;
	
	resIndex.erase(data[delIndex].object);
	index.erase(name);

	log_msg("res del " + id, "Deleting resourse " + data[delIndex].name);
	delete data[delIndex].object;

	if (delIndex + 1 < (int)data.size())
	{
		index[data.back().name] = delIndex;
		resIndex[data.back().object] = delIndex;
		data[delIndex] = data.back();
	}
	
	data.resize(data.size() - 1);
		
	return true;
}


template<class T>
T* ResCollection<T>::addForce(const std::string& name, bool pop)
{
	std::string baseDirectory;
	std::string filename;
	splitPath(name, baseDirectory, filename);
	resStack.push(baseDirectory);
	std::string fullResName = createPath(baseDirectory, filename);

	if (failedResourcesCache.find(fullResName) != failedResourcesCache.end())
	{
//		log_msg("res " + id, "Ignoring " + fullResName);
		if (pop) resStack.pop();
		return NULL;
	}

//	log_msg("res " + id, "Loading " + fullResName);

    int s = classes.size();
	for(int i = 0; i < s; i++)
	{
		T* obj = (*classes[i])(filename, baseDirectory);

		if (obj == NULL) continue;

		ResStorage newResStorage;
		newResStorage.id = obj->getId();
		newResStorage.name = fullResName;
		newResStorage.links = 1;
		newResStorage.object = obj;

		data.push_back(newResStorage);

		index[fullResName] = data.size() - 1;
		resIndex[obj] = data.size() - 1;
	
//		log_msg("res " + id, "OK " + fullResName);
		if(pop) resStack.pop();

		return obj;
	}
	log_msg("res error " + id, "Failed " + fullResName);
	if (pop) resStack.pop();

	failedResourcesCache.insert(fullResName);

	return NULL;
}

// Вернуть ресурс по имени
template<class T>
inline T* ResCollection<T>::operator[] (const std::string& name) 
{
	int i = getIndex(name);
	if (i < 0)
	{
		return NULL;
	}
	else
	{
		return data[i].object;
	}
}

template<class T>
inline int ResCollection<T>::getIndex(const std::string& name)   
{
	std::map<const std::string,int>::const_iterator it = index.find(name);
	if(it != index.end())
	{
		return it->second;	
	}
	else 
	{
		return -1;
	}
}  /*If exist - return*, esle 0 */


#endif
