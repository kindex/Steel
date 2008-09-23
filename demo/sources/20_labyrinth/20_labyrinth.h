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
#include "visibility_check.h"
#include "../objects/tag.h"

#include <objects/combiner/graph_object.h>
#include <objects/combiner/audio/simple_sound.h>
#include <enet/enet.h>

typedef std::vector<GraphObject*> GraphObjectVector;

class NxPhysicsSDK;
class NxScene;
typedef std::set<GameObject*> GameObjectSet;

enum LabyrinthSoundID { WIN, LOSS };
const int SoundCount = 2;

class GameLabyrinth: public GameFreeScene
{
public:
	GameLabyrinth();
	~GameLabyrinth();
	void handleEventKeyDown(const std::string& key);
	void handleMouse(double dx, double dy);
	bool init(Config& _conf, Input& _input);
	void process();
	void processPhysic();
    std::string getWindowCaption();
	void draw(GraphEngine&);
	void bind(GraphEngine&);
	void processAudio();

    friend class AgeiaInjector;

private:
// ------------------ Labyrinth -----------------
	Labyrinth         labyrinth;
    CellVisibilitySet cell_visibility_set;
	GraphObjectVector walls;
	ConfigArray*      wscene[2];
    v2                cell_size;
    GraphObject*      character_model;
    GameObjectSet     injected_objects;
    GameObjectSet     all_objects;
    Combiner*         floor;
    std::map<Cell, GameObjectSet> objects_cell_map;
    CellVisibilitySet current_sell_set;
    bool              createLabyrinth();
    void              updateVisibleObjects(GraphEngine&);
	//SimpleSound* winAudioObject;
	SimpleSound* lossAudioObject;
    // TODO: introduce map LabyrinthSounds
    typedef std::map<LabyrinthSoundID, SimpleSound*> LabyrinthSoundMap;
    LabyrinthSoundMap labyrinthSounds;

// -------------------- Game --------------------
    Character*      active_character;
    float           cameraPenalty;
    Character*      winner; // used when game_state == GAME_END
    CharacterVector characters;
    GameState       game_state;
    std::string     client_winner;
    TagVector       character_starts;
    std::string     game_chat;

    class FullScreenMessage : public GameObject
    {
    public:
        bool supportsInterface(IN OUT Engine&, IN const InterfaceId interface_id)
        {
            return interface_id == INTERFACE_GRAPH;
        }
        bool updateInformation(IN OUT Engine&, IN const InterfaceId);
        void process(const ProcessInfo&) {}

        bool InitFromConfig(Config&) { return false; }
        Config* getConfig() const { return NULL;}

        std::string message;
    } full_screen_message;

    std::map<uid, Character*> character_index;
    Character* findCharacter(uid character_id);
    IdGenerator character_id_generator;
    
    void addCharacter(Character* new_character);
    void deleteCharacter(Character* character);
    bool createCharacters();
    Character* createCharacterStart();
    Character* createCharacter();
    bool createPhysicWorld();
    bool isWinner(Character*);
	void checkForWinner();
    void restart();
    void processChatKey(std::string);

// ------------------- Physic ---------------------
    bool initAgeia();
    void exitAgeia();
    NxActor* ageiaCreateSurface(const GraphObject& object, const ObjectPosition&, bool _static);
    NxActor* ageiaCreateBox(const GraphObjectBox& box, const ObjectPosition&, bool _static);
    NxActor* ageiaCreateCharacter(IN OUT Character& character);
    void ageiaDeleteCharacter(Character& character);
    void ageiaInject(GameObject* object, const ObjectPosition& base_position);

	NxPhysicsSDK* physicsSDK;
	NxScene*      ageia_scene;
    v3            global_gravity;

// --------------------- Net -----------------------
	Timer         netTimerSend;
	Timer         netTimerReceive;
	Timer         netTimerReconnect;
    ENetHost*     host;
    bool          refresh_needed;

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
    
    // Server side
    bool serverInit();
	void serverProcess();
    void serverDisconnectClient(Client* client);

	void serverSendInformationToClients();
	void serverSendS_CHARACTER_UPDATE(Client* client);
    void serverSendWorld(Client* client);
    void serverSendS_INIT(Client* client);
    void serverSendS_BIND_CHAR(Client* client, size_t characterIndex);
    void serverSendS_GAME_INFO(Client* client);

    void serverSend_PONG(Client* client, const NetworkPacket::Format::Ping& pingRequest);
    void serverReceive_PING(Client* client, NetworkPacket* packet, size_t dataLength);
    void serverReceive_CHAR_UPDATE(Client* client, NetworkPacket* packet, size_t dataLength);

    void serverReceiveC_INIT(Client* client, NetworkPacket* packet, size_t dataLength);
    void serverReceiveC_WORLD_LOADED(Client* client, NetworkPacket* packet, size_t dataLength);

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
