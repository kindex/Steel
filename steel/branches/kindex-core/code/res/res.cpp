#include "res.h"

using namespace std;

void ResCollection::registerClass(Res* Class, int size, string fileextension)
{
	classes[fileextension].data = Class;
	classes[fileextension].size = size;
}

Res* ResCollection::getClass(string fileextension)
{
	Res *a = (Res*)malloc(classes[fileextension].size);
	memcpy((void*)a, classes[fileextension].data, classes[fileextension].size);
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

Res* ResCollection::addForce(const string& name)
{
    index[name] = freeindex;
    data.resize(freeindex+1);
    names.resize(freeindex+1);
    setname(freeindex, name);

	string ext = getext(name);

	data[freeindex] = getClass(ext);

	string namecopy = name;
	data[freeindex]->init(namecopy); // в последствии хочу передавать параметр из загрузки через изменённое имя

    return data[freeindex++];
}

Res* ResCollection::add(const string& name)
{
	if(index.find(name) == index.end())
	{
		return addForce(name);
	}
	else
	return data[index[name]];
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
