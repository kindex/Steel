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
#include "../utils.h"

// Resourse stream
class rstream: public std::ifstream
{
public:
	rstream(std::string s) 
	{ 
		_
		open(s.c_str(), std::ios::binary | std::ios::in); 
		_
	}
	void read(void *dest, int size);
	void skip(int n);// skip n byten in input stream
};


class Res: public steelAbstract
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

class ResCollection: public steelAbstract
{
//	typedef map<const string,int> t_index;
//	typedef t_index::value_type value_type;
//	typedef vector<string> t_names;

	std::vector<Res*> data;
	std::map<const std::string,int> index; // Ïî èìåíè âîçâðàøàåò èíäåêñ â ìàññèâå data
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
Neaao?uea 2 ooieoee caiiieia?o eeann ii eiaie e nicaa?o yecaiiey? caiiiiaiiiai eeanna.
Noaiaa?oiiai ?aoaiey ia iaoae, ii yoiio y i?inoi ?a?ac malloc+memcpy eiie?o? iauaeo
e aucuaa? aai eiino?oeoi? aua ?ac.
*/
	void registerClass(Res* Class, int size, std::string fileextension);
	Res* getClass(std::string fileextension);
};


#endif
