/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2007-2008
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Labyrinth Game main unit
 ************************************************************/

#ifndef _20_LABYRINTH_H_
#define _20_LABYRINTH_H_

#include "../game_free_scene.h"
#include "labyrinth_generator.h"
#include "character.h"

#include <objects/combiner/graph_object.h>
#include <enet/enet.h>

typedef std::vector<GraphObject*> GraphObjectVector;

class NxPhysicsSDK;
class NxScene;

class GameLabyrinth: public GameFreeScene
{
public:
	GameLabyrinth();
	~GameLabyrinth();
	void handleEventKeyDown(const std::string& key);
	void handleMouse(double dx, double dy);
	bool init(Config& _conf, Input& _input);
	void process();
	void processNetwork();
	void sendInformationToClients();
    std::string getWindowCaption();
	void draw(GraphEngine&);
    bool createWorld();
    bool createCharacter();
    bool createPhysicWorld();

    friend class AgeiaInjector;

private:
// ------------------ Labyrinth -----------------
	Labyrinth labyrinth;
	GraphObjectVector walls;
	ConfigArray* wscene[2];
	float length[2];
	int count[2];
// -------------------- Game --------------------
    Character* character;
    std::vector<Character*> characters;

    enum GameState {
        GAME_PLAYING,
        GAME_WIN,
    } gameState;

// ------------------- Physic ---------------------
    bool initAgeia();
    void exitAgeia();
    NxActor* createSurface(const GraphObject& object, const ObjectPosition&, bool _static);
    NxActor* createBox(const GraphObjectBox& box, const ObjectPosition&, bool _static);

	NxPhysicsSDK* physicsSDK;
	NxScene*      pScene;
    v3            global_gravity;

// --------------------- Net -----------------------
	Timer         netTimer;
    ENetHost*     host;
    ENetPeer*     peer;

    enum NetRole
    {
        NET_SINGLE,
        NET_SERVER,
        NET_CLIENT,
    } net_role;

    struct NetworkPacket
    {
        enum PacketKind
        {
            P_CHARACTER_UPDATE,
            P_WORLD,
        } kind;

        union
        {
            struct CharacterUpdate
            {
                ObjectPosition position;
                v3simple       linear_velocity;
                v3simple       linear_momentum;
            } character_update;
#pragma warning (push)
#pragma warning (disable : 4200)
            char world[];
#pragma warning (pop)
        } data;
    };
};

#endif
