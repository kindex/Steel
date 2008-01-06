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
#include <common/containers/string_vector.h>
#include "../svn.inc"

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
    std::string getWindowCaption();
	void draw(GraphEngine&);

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
    bool createWorld();
    bool createCharacter();
    bool createPhysicWorld();

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
    ENetPeer*     server;
    struct Client
    {
        Client(ENetPeer* peer) : peer(peer), character(NULL), state(DISCONNECTED) {}
        enum ClientState
        {
            DISCONNECTED,
            CONNECTING,
            LOADING_WORLD,
            PLAYING,
        } state;
        ENetPeer*  peer;
        Character* character;
        void disconenct();
    };
    typedef std::vector<Client*> ClientVector;
    ClientVector clients;
    Client::ClientState client_state;

    enum NetRole
    {
        NET_SINGLE,
        NET_SERVER,
        NET_CLIENT,
    } net_role;

    struct NetworkPacket
    {
        static const unsigned int PROTOCOL_VERSION = SVN_REVISION;
        enum PacketKind
        {
            S_CHARACTER_UPDATE = 0x1000,
            S_INIT,
            C_INIT,
            S_WORLD,
            C_WORLD_LOADED,
            SC_PING,
            SC_PONG,
        } kind;

        StringVector extractStrings(size_t offset, size_t total_size) const;
        union Format
        {
            struct S_CharacterUpdate
            {
                uid            characterId;
                ObjectPosition position;
                v3simple       linear_velocity;
                v3simple       linear_momentum;
            } character_update; // P_CHARACTER_UPDATE
            struct S_Init
            {
                unsigned int protocol_version;
            } init;
        } data;
    };
    size_t setupNetworkPacketStrings(NetworkPacket*& packet, const size_t size, const StringVector& strings) const;
	void processServer();
    void serverSendWorld(Client* client);
    void serverSendInit(Client* client);
	void processClient();
	void sendInformationToClients();
	void disconnectFromServer();
};

#endif
