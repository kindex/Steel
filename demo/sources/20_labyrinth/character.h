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
#include <objects/ps/particle_system.h>
#include <objects/combiner/audio/simple_sound.h>

class NxActor;
class Input;

class Character : public GameObject
{
public:
    struct CharacterPosition
    {
        ObjectPosition position;
        v3simple       linear_velocity;
        v3simple       linear_momentum;
    };

    Character();
	bool supportsInterface(IN OUT Engine&, IN const InterfaceId);
	bool updateInformation(IN OUT Engine&, IN const InterfaceId);
    void bindEngine(IN OUT Engine&, IN const InterfaceId);
	void process(const ProcessInfo&);
	bool InitFromConfig(Config&);
    Config* getConfig() const;
    void traverse(Visitor&, const ObjectPosition& base_position);

    bool isAlive() const {return alive;}
    ObjectPosition getPosition() const;
    v3 getVelocity() const;
    v3 getAngularMomentum() const;
    bool trustPosition(const CharacterPosition&) const;
    void setCharacterPosition(const CharacterPosition&);
    void setPosition(const ObjectPosition&);
    void setVelocity(const v3&);
    void setAngularMomentum(const v3&);
    void setDirection(const v3&);
    void setInput(Input*);

	void setIdleSound(SimpleSound*);
	SimpleSound* getIdleSound() const;

    enum Owner
    {
        FREE,
        SERVER,
        CLIENT
    };

    friend class GameLabyrinth;
    friend class AgeiaInjector;

private:
    float force;
    float trust_distance;
    bool alive;
    bool position_is_set;
    GraphObject*   graph_object;
    ParticleSystem* smoke;
    NxActor*       physic_object;

    ObjectPosition origin;
    v3             direction;
	Input*         input;
    uid            character_id; // synchronized on all clients
    Owner          owner;
    uid            clientId;
    std::string    name;

	// sounds
	AudioObject* idleSound;
	AudioObject* startSound;
};

typedef std::vector<Character*> CharacterVector;

#endif
