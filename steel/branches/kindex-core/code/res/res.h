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
using namespace std;

class Res
{
public:
	virtual bool init(string& name);
	virtual bool load(ifstream &f) = 0;
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

	vector<Res*> data;
	map<const string,int> index; // По имени возврашает индекс в массиве data
	vector<string> names;

	map<const string, ClassCopy> classes;

	int freeindex;
public:
	ResCollection(): freeindex(0) {}

	Res* operator [] (const int n)        { return data[n]; }
    Res* operator [] (const string& name) { return data[getindex(name)]; }

    int getindex(const string& name)
    {
        return index[name];
    }  /*If exist - return*, esle 0 */

    int lastinsertedid(){ return freeindex-1; }
    void setname(int n, string name) { index[name] = n; names[n] = name; }

	Res* addForce(const string& name);
	Res* add(const string& name);

/*
Следующие 2 функции запоминают класс по имени и создают экземпляр запомненного класса.
Стандартного решения не нашел, по этому я просто через malloc+memcpy копирую объект
и вызываю его конструктор еще раз.
*/
	void registerClass(Res* Class, int size, string fileextension);
	Res* getClass(string fileextension);
};


#endif
