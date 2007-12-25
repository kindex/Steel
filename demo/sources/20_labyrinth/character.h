/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2006-2007
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Character - object, whuch can walk in the labyrinth,
        use weapons, and be controled by human through keyboard or network.
 ************************************************************/

#ifndef _LABYRINTH_CHARACTER_H_
#define _LABYRINTH_CHARACTER_H_

#include <objects/combiner/graph_object.h>

class NxActor;

class Character : public GameObject
{
public:
    Character();
	bool supportsInterface(IN OUT Engine&, IN const InterfaceId);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId);
    void bindEngine(IN OUT Engine&, IN const InterfaceId);
	void process(const ProcessInfo&);
	bool InitFromConfig(Config&);

	void handleEventKeyDown(const std::string& key);

    float getHealth() const { return health; }
    bool isAlive() const {return alive;}
    v3 getPosition() const;

private:
    float health;
    bool alive;
    GameObject* graph_object;
    NxActor* physic_object;
    ObjectPosition position;
};

#endif
