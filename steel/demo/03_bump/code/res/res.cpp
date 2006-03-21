#include "res.h"
#include "../common/logger.h"

using namespace std;

void ResCollection::registerClass(Res* Class, int size, const Res::res_kind kind)
{
	classes[kind].push_back(ClassCopy(Class,size));
}

Res* ResCollection::createClass(ClassCopy *aclass)
{
	Res *a = (Res*)malloc(aclass->size);
	memcpy((void*)a, aclass->data, aclass->size);
	return a;
}

string getext(string name)
{
	string r;
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
			bool ret = addForce(it->kind, it->name);
			if(!ret) return false;
		}
	}
	return true;
}


/*
name - идентификатор ресурса. Обычно это имя файла без расширения, 
но может быть ипрограммно генерируемой тестурой.
Пробуем загрузить всеми доступными загрузчиками по порядку.
*/

bool ResCollection::addForce(const Res::res_kind kind, const std::string& name)
{
	for(ResClassArray::iterator it = classes[kind].begin(); it != classes[kind].end(); it++)
	{
		Res *loader = createClass(&(*it));

		if(!loader->init(name, *this)) continue;
		
		index[name] = freeindex;
	    data.resize(freeindex+1);
	    names.resize(freeindex+1);
	    resType.resize(freeindex+1);
	    setname(freeindex, name);
		data[freeindex] = loader;
		resType[freeindex] = kind;
	
		alog.out("Res: loaded %s", name.c_str());

		freeindex++;
		return true;
	}
	alog.out("Res: failed %s", name.c_str());
	return false;
}

bool ResCollection::add(const Res::res_kind kind, const std::string& name)
{
	if(index.find(name) == index.end())
	{
		return addForce(kind, name);
	}
	else
		return true;
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
	ifstream::read((char*)dest, size);
}
