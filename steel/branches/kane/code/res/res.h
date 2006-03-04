#ifndef __RES_H
#define __RES_H

/*
Õðàíåíèå ðåñóðñîâ. Ðåñóðñû áûâàþò òðžõ âèäîâ:
* Áèíàðíûå
* Ìàññèâ, èíäåêñèðîâàííûé ñòðîêàìè (õðàíåíèå íàñòðîåê)
* Ñâîÿ ñòðóêòóðà, âðîäå ìîäåëåé
Äîëæíî áûòü ïî êëàññó íà òàêèå òèïû äàííûõ, êàê:
Model, Image, Audio, Engine Settings, Animation, Weapon Settings
Îò íèõ íàñëåäóþòñÿ êëàññû ñ äåòàëèçèðîâàííûì õðàíåíèåì, íàïðèìåð:
Image: BMP, JPEG, PNG

Êàæäûé ðåñóðñ èäåíòèôèöèðóåòñÿ ïóòžì ê ôàéëó, èç êîòîðîãî îí çàãðóæàåòñÿ, èñêëþ÷àÿ
ðàñøèðåíèå ôàéëà, òàê êàê ôîðìàò õðàíåíèÿ íå äîëæåí óêàçûâàòüñÿ â ññóëêàõ íà ýòîò ôàéë.
Ïî èäåå, ðåñóðñ ìîæåò è íè÷åãî íå çàãðóæàòü - à áûòü ÷èñòî ïðîãðàììíîé ýìóëÿöèåé, âðîäå 
âèäåî-òåêñòóðû??

ResCollection ñëóæèò õðàíèëèùåì äëÿ âñåõ ðåñóðñîâ, âûçûâàåò ìåòîäû äëÿ çàãðóçêè, âûãðóçêè,
åñëè ýòî íåîáõîäèìî äëÿ îò÷èñòêè ïàìÿòè.
*/

#include <map>
#include <vector>
#include <string>

#include "../common/types.h"
#include "../steel.h"

using namespace std;

class Res: public steelAbstract
{
public:
	virtual ~Res() {}
	virtual bool load(string& name) = 0;
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
	map<const string,int> index; // Ïî èìåíè âîçâðàøàåò èíäåêñ â ìàññèâå data
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
Ñëåäóþùèå 2 ôóíêöèè çàïîìèíàþò êëàññ ïî èìåíè è ñîçäàþò ýêçåìïëÿð çàïîìíåííîãî êëàññà.
Ñòàíäàðòíîãî ðåøåíèÿ íå íàøåë, ïî ýòîìó ÿ ïðîñòî ÷åðåç malloc+memcpy êîïèðóþ îáúåêò
è âûçûâàþ åãî êîíñòðóêòîð åùå ðàç.
*/
	void registerClass(Res* Class, int size, string fileextension);
	Res* getClass(string fileextension);
};


#endif
