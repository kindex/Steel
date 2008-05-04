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
#include "network_protocol.h"

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
    GraphObject*    character_model;
// -------------------- Game --------------------
    Character*      character;
    float           cameraPenalty;
    struct Client;
    Client*         winner; // used when game_state == GAME_END, NULL means server
    CharacterVector characters;
    GameState       game_state;
    std::string     client_winner;
    
    bool createWorld();
    bool createCharacter();
    bool createPhysicWorld();
    bool isWinner(Character*);
	void checkForWinner();

// ------------------- Physic ---------------------
    bool initAgeia();
    void exitAgeia();
    NxActor* createSurface(const GraphObject& object, const ObjectPosition&, bool _static);
    NxActor* createBox(const GraphObjectBox& box, const ObjectPosition&, bool _static);
    NxActor* createCharacter(IN OUT Character& character);

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
        Client(ENetPeer* peer) : 
            peer(peer),
            character(NULL),
            state(DISCONNECTED),
            clientId(objectIdGenerator.genUid())
        {}

        void disconenct();
        std::string getNetworkName() const;

        ClientState state;
        ENetPeer*   peer;
        Character*  character;
        std::string name;
        uid         clientId;
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

    size_t setupNetworkPacketStrings(NetworkPacket*& packet, const size_t size, const StringVector& strings) const;
    size_t setupNetworkPacketString(NetworkPacket*& packet, const size_t size, const std::string& string) const;
    
    // Server side
    bool serverInit();
	void serverProcess();
    void serverDisconnectClient(Client* client);

	void serverSendInformationToClients();
    void serverSendWorld(Client* client);
    void serverSendS_INIT(Client* client);
    void serverSendS_BIND_CHAR(Client* client, size_t characterIndex);
    void serverSend_PONG(Client* client, const NetworkPacket::Format::Ping& pingRequest);
    void serverSendS_GAME_INFO(Client* client);

    void serverReceiveC_INIT(Client* client, NetworkPacket* packet, size_t dataLength);
    void serverReceiveC_WORLD_LOADED(Client* client, NetworkPacket* packet, size_t dataLength);
    void serverReceive_PING(Client* client, NetworkPacket* packet, size_t dataLength);
    void serverReceive_CHAR_UPDATE(Client* client, NetworkPacket* packet, size_t dataLength);

    // Client side
    bool clientInit();
    bool clientConnectToServer();
	void clientProcess();
	void clientDisconnectFromServer();

    void clientSendInformationToServer();
    void clientSendC_INIT();
    void clientSendC_WORLD_LOADED();
    void clientSend_PING();

    void clientReceiveS_INIT(NetworkPacket* packet, size_t dataLength);
    void clientReceiveS_WORLD(NetworkPacket* packet, size_t dataLength);
    void clientReceive_CHAR_UPDATE(NetworkPacket* packet, size_t dataLength);
    void clientReceiveS_BIND_CHAR(NetworkPacket* packet, size_t dataLength);
    void clientReceive_PONG(NetworkPacket* packet, size_t dataLength);
    void clientReceiveS_GAME_INFO(NetworkPacket* packet, size_t dataLength);
};

#define net_assert(expr) { if (!(expr)) { error("net", "illegal packet"); return; } }

#endif
