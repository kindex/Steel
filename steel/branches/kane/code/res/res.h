#ifndef __RES_H
#define __RES_H

/*
O?aiaiea ?ano?nia. ?ano?nu auaa?o o?zo aeaia:
* Aeia?iua
* Iannea, eiaaene?iaaiiue no?ieaie (o?aiaiea iano?iae)
* Naiy no?oeoo?a, a?iaa iiaaeae
Aie?ii auou ii eeanno ia oaeea oeiu aaiiuo, eae:
Model, Image, Audio, Engine Settings, Animation, Weapon Settings
Io ieo ianeaao?ony eeannu n aaoaeece?iaaiiui o?aiaieai, iai?eia?:
Image: BMP, JPEG, PNG

Ea?aue ?ano?n eaaioeoeoe?oaony ioozi e oaeeo, ec eioi?iai ii caa?o?aaony, enee??ay
?anoe?aiea oaeea, oae eae oi?iao o?aiaiey ia aie?ai oeacuaaouny a nnoeeao ia yoio oaee.
Ii eaaa, ?ano?n ii?ao e ie?aai ia caa?o?aou - a auou ?enoi i?ia?aiiiie yioeyoeae, a?iaa 
aeaai-oaenoo?u??

ResCollection neo?eo o?aieeeuai aey anao ?ano?nia, aucuaaao iaoiau aey caa?ocee, aua?ocee,
anee yoi iaiaoiaeii aey io?enoee iaiyoe.
*/

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
	rstream(std::string s) 
	{ 
		open(s.c_str(), std::ios::binary | std::ios::in); 
	}
	void read(void *dest, int size);
	void skip(int n);// skip n byten in input stream
};


class Res: public steelAbstract
{
public:
	typedef enum 
	{
			none,
			image,
			model
	}	res_kind;

	struct ResLocator
	{
		std::string name;
		res_kind kind;
	};

	typedef
		std::vector<ResLocator>
		ResLocatorArray;

#define RES_KIND_COUNT 3

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

class ResCollection: public steelAbstract
{
//	typedef map<const string,int> t_index;
//	typedef t_index::value_type value_type;
//	typedef vector<string> t_names;

	std::vector<Res*> data; // resources

	std::map<const std::string,int> index; // Ïî èìåíè âîçâðàøàåò èíäåêñ â ìàññèâå data
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
    Res* operator [] (const std::string& name) { return data[getindex(name)]; }

    int getindex(const std::string& name)
    {
        return index[name];
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
