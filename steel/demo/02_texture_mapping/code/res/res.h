#ifndef __RES_H
#define __RES_H


#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "../common/types.h"
#include "../steel.h"

// Resourse stream
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


class Res: public steelAbstract
{
public:
#define RES_KIND_COUNT 4
	typedef enum 
	{
			none,
			image,
			model,
			material
	}	res_kind;

	struct ResLocator
	{
		res_kind kind;
		std::string name;

		ResLocator(res_kind akind, std::string aname): kind(akind), name(aname) {}
	};

	typedef
		std::vector<ResLocator>
		ResLocatorArray;



	virtual bool init(const std::string name, ResLocatorArray &loadBefore, ResLocatorArray &loadAfter) = 0;
//	virtual bool load(rstream &f, int size) = 0;
	virtual bool unload() = 0;
//	virtual bool reload() = 0; // reload image on driver change
};


struct ClassCopy
{
	ClassCopy(Res* adata, int asize): data(adata), size(asize) {}

	Res* data;
	int size;
};

class ResCollection
{
//	typedef map<const string,int> t_index;
//	typedef t_index::value_type value_type;
//	typedef vector<string> t_names;

	std::vector<Res*> data; // resources
	std::vector<Res::res_kind> resType;

	std::map<const std::string,int> index; // ѕо имени возврашает индекс в массиве data
	std::vector<std::string> names;

	typedef 
		std::vector<ClassCopy> 
		ResClassArray;

	
	ResClassArray classes[RES_KIND_COUNT];

	int freeindex;
public:
	ResCollection(): freeindex(0) {}

	Res* operator [] (const int n)        { return data[n]; }
	bool find(const std::string& name) {return index.find(name) != index.end(); } 
    
	Res* operator [] (const std::string& name) 
	{
		Res::res_kind kind;
		return get(name, kind);
	}

	Res* get(const std::string& name, Res::res_kind &kind) 
	{ 
		int i = getIndex(name);
		if(i<0)
		{
			kind = Res::none;
			return NULL;
		}
		else
		{
			kind = resType[i];
			return data[i]; 
		}
}

    int getIndex(const std::string name)
    {
		if(index.find(name) != index.end())
			return index[name];
		else
			return -1;
    }  /*If exist - return*, esle 0 */

    int lastinsertedid(){ return freeindex-1; }
    void setname(int n, std::string name) { index[name] = n; names[n] = name; }

	bool addForce(const Res::res_kind kind, const std::string& name);
	bool add(const Res::res_kind kind, const std::string& name);

	bool add(Res::ResLocatorArray &names);

/*
Neaao?uea 2 ooieoee caiiieia?o eeann ii eiaie e nicaa?o yecaiiey? caiiiiaiiiai eeanna.
Noaiaa?oiiai ?aoaiey ia iaoae, ii yoiio y i?inoi ?a?ac malloc+memcpy eiie?o? iauaeo
e aucuaa? aai eiino?oeoi? aua ?ac.
*/
	void registerClass(Res* Class, int size, const Res::res_kind kind);
	Res* createClass(ClassCopy *aclass);
};


#endif
