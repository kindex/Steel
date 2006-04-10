/*id*******************************************************************************
    Unit: Res [Resources]
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
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

	ResCollection res;
	res.registerClass(createBMP,	Res::image);

	res.add(Res::image, "box");

	res["Image"]->bitmap ... 

-----------------------------------------------------------------------*/


#ifndef __RES_H
#define __RES_H

#include <map>
#include <vector>
#include <string>
#include <fstream>

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
	rstream(std::string s, ios_base::openmode _Mode = std::ios::binary) 
	{ 
		open(s.c_str(), _Mode | std::ios::in); 
	}
	void read(void *dest, int size);
	void skip(int n);// skip n byten in input stream
};

// Forward declaration
class ResCollection;

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
class Res: public steelAbstract
{
public:
// количестко типов ресурсов
#define RES_KIND_COUNT 5
// типы ресурсов (типы хранения)
	typedef enum 
	{
			none,
			image,
			model,
			config,
			script
	}	res_kind;
// Структура для идентификации ресурса (тип, строка)
	struct ResLocator
	{
		res_kind kind;
		std::string name;

		ResLocator(res_kind akind, std::string aname): kind(akind), name(aname) {}
	};

	typedef
		std::vector<ResLocator>
		ResLocatorArray;

//	virtual bool init(const std::string name, ResCollection &res) = 0;
	virtual bool unload() = 0;
};

// тип: функция для геренирования копии класса, унаследованного от Res
typedef Res*(funcCreateResClass)(const std::string filename, ResCollection *res); 
// копия класса. Нужна для ассоциирования класса с типом ресурса.
struct ClassCopy
{
	ClassCopy(funcCreateResClass* _func){ func = _func;}

	funcCreateResClass* func;
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
class ResCollection
{
// Массив с классами, в которых хранятся ресурсы
	std::vector<Res*> data;
// Типы этих ресурсов
	std::vector<Res::res_kind> resType;
// Map: resource name->index
// По имени возврашает индекс в массивах data и resType
	std::map<const std::string,int> index; 
	std::vector<std::string> names;

	typedef 
		std::vector<ClassCopy> 
		ResClassArray;
// Массивы классов для рагрузки ресурсов каждого типа
	ResClassArray classes[RES_KIND_COUNT];
// Количество рагруженных ресурсов (в начале 0)
	int freeindex;
public:
	ResCollection(): freeindex(0) {}
// Вернуть ресурс по номеру
	Res* operator [] (const int n)        { return data[n]; }
// Проверить, существует ли ресурс с указанным именем
	bool find(const std::string& name) {return index.find(name) != index.end(); } 
// Найти номер ресурса по имени
    int getIndex(const std::string name)   {if(index.find(name) != index.end())	return index[name];	else return -1;  }  /*If exist - return*, esle 0 */
    
// Вернуть ресурс по имени
	Res* operator [] (const std::string& name) 
	{
		int i = getIndex(name);
		if(i<0)
			return NULL;
		else
			return data[i];
	}

// Вернуть ресурс по имени и типу
	Res* get(const Res::res_kind kind, const std::string& name) 
	{ 
		int i = getIndex(name);
		if(i<0)
			return NULL;
		else
		{
			if(kind == resType[i])
				return data[i]; 
			else
				return NULL;
		}
	}

// Вернуть ресурс (модель) по имени 
// TODO: не красиво - оптимизация
	Res* getModel(const std::string& name)
	{
		Res* m = add(Res::model, name);
		if(m != NULL)
			return m;
		else
		{
			alog.msg("error res model", "Model not found: "+name);
			return NULL;
		}
	}

// Добавить ресурс по имени и типу
	Res* addForce(const Res::res_kind kind, const std::string& name);
// Добавить ресурс по имени и типу, если ресурса еще нет в коллекции
	Res* add(const Res::res_kind kind, const std::string& name);
// Добавить массив ресурсов
	bool add(Res::ResLocatorArray &names);

// Ассоциировать тип ресурса и загрузчик.
// _func - функция для создания класса для загрузки (унаследоманного от Res)
	void registerClass(funcCreateResClass *_func, const Res::res_kind kind);
// Вернуть копию класса
	Res* createClass(ClassCopy *aclass, std::string name);
};

#endif
