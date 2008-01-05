/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2007
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
class Input;

class Character : public GameObject
{
public:
    Character();
	bool supportsInterface(IN OUT Engine&, IN const InterfaceId);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId);
    void bindEngine(IN OUT Engine&, IN const InterfaceId);
	void process(const ProcessInfo&);
	bool InitFromConfig(Config&);
    Config* getConfig() const;
    void traverse(Visitor&, const ObjectPosition& base_position);

    float getHealth() const { return health; }
    bool isAlive() const {return alive;}
    const ObjectPosition& getPosition() const;
    v3 getVelocity() const;
    v3 getMomentum() const;
    void setPosition(const ObjectPosition&);
    void setVelocity(const v3&);
    void setMomentum(const v3&);
    void setDirection(const v3&);
    void setInput(Input*);

    friend class GameLabyrinth;
    friend class AgeiaInjector;

private:
    float health;
    float force;
    bool alive;
    GameObject*    graph_object;
    NxActor*       physic_object;
    ObjectPosition position;
    ObjectPosition origin;
    v3             direction;
	Input*         input;
};

#endif
