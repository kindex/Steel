#ifndef __RES_H
#define __RES_H

/*
Хранение ресурсов. Ресурсы бывают трёх видов:
* Бинарные
* Массив, индексированный строками (хранение настроек)
* Своя структура, вроде моделей
Должно быть по классу на такие типы данных, как:
Model, Image, Audio, Engine Settings, Animation, Weapon Settings
От них наследуются классы с детализированным хранением, например:
Image: BMP, JPEG, PNG

Каждый ресурс идентифицируется путём к файлу, из которого он загружается, исключая
расширение файла, так как формат хранения не должен указываться в ссулках на этот файл.
По идее, ресурс может и ничего не загружать - а быть чисто программной эмуляцией, вроде 
видео-текстуры??

ResCollection служит хранилищем для всех ресурсов, вызывает методы для загрузки, выгрузки,
если это необходимо для отчистки памяти.
*/

#include <map>
#include <vector>
#include <string>
#include <fstream>

// Resourse stream
class rstream: public std::ifstream
{
public:
	rstream(std::string s) 
	{ 
		open(s.c_str(), std::ios::binary | std::ios::in); 
	}
/*	bool good();
	bool bad();*/
	void read(void *dest, int size);
	void skip(int n);// skip n byten in input stream
};

class Res
{
public:
	virtual bool init(std::string& name) = 0;
//	virtual bool load(rstream &f, int size) = 0;
	virtual bool unload() = 0;
//	virtual bool reload() = 0; // reload image on driver change
};

struct ClassCopy
{
	Res* data;
	int size;
};

class ResCollection
{
//	typedef map<const string,int> t_index;
//	typedef t_index::value_type value_type;
//	typedef vector<string> t_names;

	std::vector<Res*> data;
	std::map<const std::string,int> index; // По имени возврашает индекс в массиве data
	std::vector<std::string> names;

	std::map<const std::string, ClassCopy> classes;

	int freeindex;
public:
	ResCollection(): freeindex(0) {}

	Res* operator [] (const int n)        { return data[n]; }
    Res* operator [] (const std::string& name) { return data[getindex(name)]; }

    int getindex(const std::string& name)
    {
        return index[name];
    }  /*If exist - return*, esle 0 */

    int lastinsertedid(){ return freeindex-1; }
    void setname(int n, std::string name) { index[name] = n; names[n] = name; }

	Res* addForce(const std::string& name);
	Res* add(const std::string& name);

/*
Следующие 2 функции запоминают класс по имени и создают экземпляр запомненного класса.
Стандартного решения не нашел, по этому я просто через malloc+memcpy копирую объект
и вызываю его конструктор еще раз.
*/
	void registerClass(Res* Class, int size, std::string fileextension);
	Res* getClass(std::string fileextension);
};


#endif
