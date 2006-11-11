/*id*******************************************************************************
	File: res/res.h
	Unit: res
	Part of: Steel engine
	(C) DiVision, 2003-2006
	Authors:
		* KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
	License:
		Steel Engine License
	Description:
        Молуль для хранения, загрузки игровых ресурсов и контроля над ними. 
	Parts:
		res.cpp
		В поддиректориях хранятся классы для загрузки и хранения всех типов ресурсов.
**************************************************************************************/

#ifndef __RES_H
#define __RES_H

#include "../steel.h"

#include <map>
#include <string>
#include <stack>

#include "../common/types.h"
#include "../common/utils.h"
#include "../common/logger.h"

#include "../engine/id_generator.h"


// Собирает полное имя файла относительно директории. Если имя файла начинается с /, то имя файла считается уже полным
std::string getFullPath(std::string filename, std::string directory);

// Стек текущих директорий. Используется только коллекцией ресурсов.
class ResStack
{
protected:
	int level;
	std::stack<std::string> stack;

public:
	bool	push(std::string directory);
	bool	pushFullPath(std::string path)
	{
		std::string baseDirectory;		
		splitPath(path, baseDirectory, path);
		return push(baseDirectory);
	}

	bool	pop(void);
	int		getLevel(void);
	std::string top(void);

	std::string getFullName(const std::string name)
	{
		return getFullPath(name, top());
	}

	template<class T>
	friend class ResCollection;

	ResStack(): level(0) {}
};
extern ResStack resStack;

/*
	Класс - храниель и рагрузчик одного ресурса.
	Хранение - стандартизовано для каждого типа ресурса. Загруска - для каждого типа может быть несколько классов для загрузки.

	От этого класса наследуются классы для хранения ресурсов для каждого типа (image, model).
	В них реализуется деструктор, который уничтожает структуры, созданные при загрузке.
	От них наследуются загрузчики, которые переопределяют метод init.
	init загружает ресурс или генерирует по строковому идентификатору.
*/
class Res//: public virtual BufferedElement
{
protected:
	uid resId; // уникальный идентификатор
public:
//	virtual bool init(const std::string name, const std::string dir) = 0;

	uid	getId(void) { return resId; }
	void setId(uid id) { resId = id; }
	virtual ~Res(void) {}
};


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
	ResCollection(void) {}

	inline T* operator[] (const std::string& name);// Вернуть ресурс по полному имени
	inline T* get(const std::string& name) { return operator[](name); }  // Вернуть ресурс по полному имени

	// Добавить ресурс по имени и типу, если ресурса еще нет в коллекции
	T* add(const std::string name, bool pop = true);

	// Удалить ресурс
	bool remove(const std::string name);
	bool remove(T* object);

	// У процедуры add есть второй параметр типа bool. Если он равняется false, то после загрузки ресурса текущая директория не восстанавливается и надо это делать вручную с помощью вызова pop.
	void pop(void) { resStack.pop(); }


protected:
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
	steel::vector<ResStorage> data; // массив хранимых ресурсов и дополнительная информация к каждому ресурсу.

	// тип: функция для геренирования копии класса, унаследованного от Res
	typedef T*(funcCreateResClass)(const std::string filename, const std::string baseName); 

// Массивы классов для рагрузки ресурсов каждого типа
	steel::vector<funcCreateResClass*> classes;

	// *********** Functions ***************
	bool removeRaw(int index); // удаляет ссылку на ресурс. Если количество ссылок = 0, то ресурс удалается физически из коллекции
	virtual bool purge(int delIndex); // физически удаляет ресурс из коллекции

	// Найти номер ресурса по полному имени
    inline int getIndex(const std::string name);
	// Проверить, существует ли ресурс с указанным полным именем
	inline bool find(const std::string& name) {return index.find(name) != index.end(); } 

	virtual T* addForce(std::string name, bool pop = true);

public:
	// Добавляет функцию для создания класса, котоыре умеет загружать ресурс
	void registerResLoader(funcCreateResClass *_func)	{		classes.push_back(_func);	}
	void setId(std::string _id) { id = _id; } // устанавливает идентификатор коллекии
};



template<class T>
T* ResCollection<T>::add(const std::string name, bool pop)
{
	std::string name2 = resStack.getFullName(name);
	int index = getIndex(name2);

	if(index < 0)
	{
		return addForce(name2, pop);
	}
	else
	{
		data[index].links++;

		if(!pop)
			resStack.pushFullPath(name2);

		return data[index].object;
	}
}

template<class T>
bool ResCollection<T>::remove(T* object)
{
#if STEEL_COMPILER == COMPILER_VS8
    std::map<T*,int>::const_iterator it = resIndex.find(object);
     
	if(it == resIndex.end())
		return false;
	else 
		return removeRaw(it->second);
#else
// TODO
	if(resIndex.find(object) == resIndex.end())
		return false;
	else 
		return removeRaw(resIndex.find(object)->second);
#endif
}


template<class T>
bool ResCollection<T>::remove(const std::string name)
{
	int index = getIndex(name);
	if(index < 0)
		return false;
	else
		return removeRaw(index);
}

template<class T>
bool ResCollection<T>::removeRaw(int index)
{
	if(data[index].links > 0)
	{
		data[index].links--;
		if(data[index].links == 0)
		purge(index);
		return true;
	}
	else
		return false;// уже удалён
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

	if(delIndex + 1 < (int)data.size())
	{
		index[data.back().name] = delIndex;
		resIndex[data.back().object] = delIndex;
		data[delIndex] = data.back();
	}
	
	data.resize(data.size() - 1);
		
	return true;
}


template<class T>
T* ResCollection<T>::addForce(std::string name, bool pop)
{
	std::string baseDirectory;
	splitPath(name, baseDirectory, name);
	resStack.push(baseDirectory);
	std::string fullResName = createPath(baseDirectory, name);

	log_msg("res " + id, "Loading " + fullResName);

    int s = classes.size();
	for(int i = 0; i < s; i++)
	{
		T *obj = (*classes[i])(name, baseDirectory);

		if(obj == NULL) continue;

		ResStorage newResStorage;
		newResStorage.id = obj->getId();
		newResStorage.name = fullResName;
		newResStorage.links = 1;
		newResStorage.object = obj;

		data.push_back(newResStorage);

		index[fullResName] = data.size() - 1;
		resIndex[obj] = data.size() - 1;
	
		log_msg("res " + id, "OK " + fullResName);
		if(pop) resStack.pop();

		return obj;
	}
	log_msg("res error " + id, "Failed " + fullResName);
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
		return data[i].object;
}

template<class T>
inline int ResCollection<T>::getIndex(const std::string name)   
{
	std::map<const std::string,int>::const_iterator it = index.find(name);
	if(it != index.end())
		return it->second;	
	else 
		return -1;
}  /*If exist - return*, esle 0 */


#endif
