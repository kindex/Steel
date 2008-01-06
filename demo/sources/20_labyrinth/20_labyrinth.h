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

    enum ClientState
    {
        DISCONNECTED,
        CONNECTING,
        LOADING_WORLD,
        PLAYING,
    };
// for client
    struct Server
    {
        Server(): 
            peer(NULL), 
            client_state(DISCONNECTED),
            ping(-1.0f),
            lastPingRequest(-1.0f),
            lastConnectTry(-1.0f)
        {} 
        std::string getNetwornName() const;

        ENetPeer*   peer;
        steel::time ping;
        steel::time lastPingRequest;
        steel::time lastConnectTry;
        ClientState client_state;
    } server;

// for server
    struct Client
    {
        Client(ENetPeer* peer) : peer(peer), character(NULL), state(DISCONNECTED) {}
        void disconenct();
        std::string getNetworkName() const;

        ClientState state;
        ENetPeer*   peer;
        Character*  character;
        std::string name;
    };
    typedef std::vector<Client*> ClientVector;
    ClientVector clients;
// common
    enum NetRole
    {
        NET_SINGLE,
        NET_SERVER,
        NET_CLIENT,
    } net_role;

    struct NetworkPacket
    {
        typedef unsigned int ProtocolVersion;
        static const ProtocolVersion PROTOCOL_VERSION = SVN_REVISION;
        enum PacketKind
        {
            S_CHARACTER_UPDATE = 0x1000,
            S_INIT,
            C_INIT,
            S_WORLD,
            C_WORLD_LOADED,
            PING,
            PONG,
        } kind;

        StringVector extractStrings(size_t offset, size_t total_size) const;
        std::string extractString(size_t offset, size_t total_size) const;
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
                ProtocolVersion protocol_version;
            } s_init;
            struct C_Init
            {
                ProtocolVersion protocol_version;
            } c_init;
            struct C_WorldLoaded
            {
            } c_worldLoaded;
            struct Ping
            {
                steel::time timestamp;
            } ping;
            struct Pong
            {
                steel::time timestamp;
            } pong;
        } data;
    }; // struct NetworkPacket
    size_t setupNetworkPacketStrings(NetworkPacket*& packet, const size_t size, const StringVector& strings) const;
    size_t setupNetworkPacketString(NetworkPacket*& packet, const size_t size, const std::string& string) const;
    
    // Server side
    bool serverInit();
	void serverProcess();
    void serverDisconnectClient(Client* client);

	void serverSendInformationToClients();
    void serverSendWorld(Client* client);
    void serverSendInit(Client* client);
    void serverSend_PONG(Client* client, const NetworkPacket::Format::Ping& pingRequest);

    void serverReceiveC_INIT(Client* client, NetworkPacket* packet, size_t dataLength);
    void serverReceiveC_WORLD_LOADED(Client* client, NetworkPacket* packet, size_t dataLength);
    void serverReceive_PING(Client* client, NetworkPacket* packet, size_t dataLength);

    // Client side
    bool clientInit();
    bool clientConnectToServer();
	void clientProcess();
	void clientDisconnectFromServer();

    void clientSendC_INIT();
    void clientSendC_WORLD_LOADED();
    void clientSend_PING();

    void clientReceiveS_INIT(NetworkPacket* packet, size_t dataLength);
    void clientReceiveS_WORLD(NetworkPacket* packet, size_t dataLength);
    void clientReceiveS_CHARACTER_UPDATE(NetworkPacket* packet, size_t dataLength);
    void clientReceive_PONG(NetworkPacket* packet, size_t dataLength);
};

#define net_assert(expr) { if (!(expr)) { error("net", "illegal packet"); return; } }

#endif
