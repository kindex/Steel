/*id*********************************************************
    Unit: Res [Resources]
    Part of: Steel engine
    Version: 1.0
    Authors:
        * KindeX [Andrey Ivanov, kindex@kindex.lv, http://kindex.lv]
    Licence:
        Только для Division
    Description:
        Молуль для хранения, загрузки игровых ресурсов и 
		контроля над ними. 
************************************************************/

#include "res.h"
#include "../common/logger.h"

void ResCollection::registerClass(funcCreateResClass *_func, const Res::res_kind kind)
{
	classes[kind].push_back(ClassCopy(_func));
}

Res* ResCollection::createClass(ClassCopy *aclass, std::string name)
{
	return aclass->func(name, this);
}

std::string getext(std::string name)
{
	std::string r;
	for(int i=name.length()-1; i>=0; i--)
		if(name[i]!='.')
			r = name[i] + r;
		else
			break;
	return r;
}

bool ResCollection::add(Res::ResLocatorArray &names)
{
	for(Res::ResLocatorArray::iterator it = names.begin();
		it != names.end();
		it++)
	{
		if(!find(it->name))
		{
			if(!addForce(it->kind, it->name)) return false;
		}
	}
	return true;
}


/*
name - идентификатор ресурса. Обычно это имя файла без расширения, 
но может быть ипрограммно генерируемой тестурой.
Пробуем загрузить всеми доступными загрузчиками по порядку.
*/

Res* ResCollection::addForce(const Res::res_kind kind, const std::string& name)
{
	for(ResClassArray::iterator it = classes[kind].begin(); it != classes[kind].end(); it++)
	{
		Res *obj = createClass(&(*it), name);
		if(obj == NULL) continue;
		
		index[name] = freeindex;
	    data.resize(freeindex+1);
	    names.resize(freeindex+1);
	    resType.resize(freeindex+1);
	    
		index[name] = freeindex; 
		names[freeindex] = name; 

		data[freeindex] = obj;
		resType[freeindex] = kind;
	
		alog.out("Res: loaded %s", name.c_str());

		freeindex++;
		return data[freeindex-1];
	}
	alog.out("Res: failed %s", name.c_str());
	return NULL;
}

Res* ResCollection::add(const Res::res_kind kind, const std::string& name)
{
	if(index.find(name) == index.end())
	{
		return addForce(kind, name);
	}
	else
		return get(kind, name);
}


/*bool Res::init(string& name)
{
	rstream f("../res/"+name);

	if(!f.good()) return false;
	if(!this->load(f, 0)) return false;
	return !f.bad();
}*/

//TEMP
#define bufsize 1024
char buf[bufsize];

void rstream::skip(int n)
{
	while(n>0)
	{
		int m =n;
		if(m > bufsize) m = bufsize;
		read(buf, m);
		n -= m;
	}
}

/*rstream::rstream(string s)
{
	f = fopen(s.c_str(), "rb");
	if(f)
		ok = true;
	else
		ok = false;
}*/

void rstream::read(void *dest, int size)
{
	std::ifstream::read((char*)dest, size);
}

bool rstream::open(std::string s, std::string ext, ios_base::openmode _Mode) 
{ 
	std::string r = std::string("../res/") + s + (ext == ""?"":"." + ext);

	std::ifstream::open(r.c_str(), _Mode | std::ios::in);
	if(fail())
	{
		std::ifstream::open((std::string("../res/") + s).c_str(), _Mode | std::ios::in);
		if(fail())
			return false;
	}
	return true;
}
