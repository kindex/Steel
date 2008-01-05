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

#include <enet/enet.h>
#include "20_labyrinth.h"
#include <common/logger.h>
#include <engine/visitor.h>
#include <res/res_main.h>
#include <res/config/config_parser.h>
#include <objects/ps/particle_system.h>
#include <objects/combiner/combiner.h>
#include <math/plane.h>

#include <NxPhysics.h>
#include <NxCooking.h>
#include <Stream.h>
#include "../23_ageia_tech/error_stream.h"
#include "steel_nx.h"

struct CharacterCollector : public Visitor
{
    bool visit(IN OUT GameObject* object)
    {
        if (dynamic_cast<Character*>(object) != NULL)
        {
            characters.push_back(dynamic_cast<Character*>(object));
        }
        return true;
    }

    std::vector<Character*> characters;
};

class AgeiaInjector : public Visitor
{
public:
    AgeiaInjector(GameLabyrinth& game) : 
        game(game),
        inChar(0)
    {}

private:
    bool visit(IN OUT GameObject* object)
    {
        bool is_char = dynamic_cast<Character*>(object) != NULL;
        if (is_char || inChar > 0)
        {
            inChar++;
        }

        return true;
    }

    void postvisit(IN OUT GameObject* object, const ObjectPosition& base_position)
    {
        if (dynamic_cast<GraphObject*>(object) != NULL)
        {
            NxActor* actor = NULL;
            if (dynamic_cast<GraphObjectBox*>(object) != NULL)
            {
                actor = game.createBox(*dynamic_cast<GraphObjectBox*>(object), base_position, inChar == 0);
            }
            else
            {
                actor = game.createSurface(*dynamic_cast<GraphObject*>(object), base_position, inChar == 0);
            }
            if (inChar != 0 && game.character != NULL)
            {
                actor->userData = (void*)(1);
                game.character->physic_object = actor;
            }
        }
        if (inChar > 0)
        {
            inChar--;
        }
    }

    GameLabyrinth& game;
    int inChar;
};

GameLabyrinth::GameLabyrinth():
    character(NULL),
    gameState(GAME_PLAYING),
    host(NULL),
    peer(NULL),
    net_role(NET_SINGLE),
    pScene(NULL),
    physicsSDK(NULL)
{}

bool GameLabyrinth::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}

	cameraMode = C_FIXED;

	if (!initAgeia())
	{
		abort_init("ageia", "Cannot init Ageia");
		return false;
	}

// network
    std::string net_role_str = conf->gets("net.role", "single");
    if (net_role_str == "single")
    {
        net_role = NET_SINGLE;
        if (!createWorld())
        {
            return false;
        }
        createCharacter();
    }
    else if (net_role_str == "server")
    {
        net_role = NET_SERVER;
        if (enet_initialize() != 0)
        {
            abort_init("net", "An error occurred while initializing ENet");
        }

        ENetAddress address;
        address.host = ENET_HOST_ANY;
        address.port = conf->geti("net.port", 2007);

        host = enet_host_create(&address, 32, 0, 0);
        if (host == NULL)
        {
            abort_init("net", "An error occurred while trying to create an ENet server host");
        }
        log_msg("net", "Listening server at port " + IntToStr(address.port));

        if (!createWorld())
        {
            return false;
        }
        createCharacter();
        netTimer.start();

// ------------------------------- TEST CLIENT-SERVER WORLD INIT
                    //character = NULL;
                    //Config* worldConfig = world->getConfig();
                    //std::string world_config_serialized = worldConfig->Dump();

                    //    Config* new_world = parseConfig(world_config_serialized);
                    //    world = static_cast<Combiner*>(createGameObject(new_world));
                    //    createCharacter();
                    //    if (graphEngine != NULL)
                    //    {
                    //        graphEngine->clear();
                    //        graphEngine->inject(world);
                    //    }
// ------------------------------- TEST CLIENT-SERVER WORLD INIT
    }
    else if (net_role_str == "client")
    {
        net_role = NET_CLIENT;
        if (enet_initialize() != 0)
        {
            abort_init("net", "An error occurred while initializing ENet");
        }

        host = enet_host_create(NULL, 1, 0, 0);
        if (host == NULL)
        {
            abort_init("net", "An error occurred while trying to create an ENet client host");
        }

        ENetAddress address;
        ENetEvent event;

        /* Connect to some.server.net:1234. */
        std::string remote_addr = conf->gets("net.remote_addr", "localhost");
        int timeout = conf->geti("net.timeout", 1000);
        enet_address_set_host(&address, remote_addr.c_str());
        address.port = conf->geti("net.port", 2007);

        /* Initiate the connection, allocating the two channels 0 and 1. */
        peer = enet_host_connect(host, &address, 2);
        
        if (peer == NULL)
        {
           abort_init("net", "No available peers for initiating an ENet connection");
        }
        
        if (enet_host_service (host, &event, timeout) > 0 &&
            event.type == ENET_EVENT_TYPE_CONNECT)
        {
            log_msg("net", "Connection to " + remote_addr + ":" + IntToStr(address.port)+ " succeeded");
        }
        else
        {
            enet_peer_reset(peer);
            abort_init("net", "Connection to " + remote_addr + ":" + IntToStr(address.port)+ " failed.");
        }

        character = NULL;
    }
    else
    {
		abort_init("net", "Unknown net role '" + net_role_str + "'");
    }
    netTimer.start();

    if (character != NULL)
    {
        character->input = input;
    }
    spectator.camera.setDirection(v3(1,1,-0.3f));

    createPhysicWorld();

    return true;
}

GameLabyrinth::~GameLabyrinth()
{
    if (host != NULL)
    {
        if (peer != NULL)
        {
            log_msg("net", "Disconnecting client " + IntToStr(peer->address.host) + ":" + IntToStr(peer->address.port));
            enet_peer_disconnect_now(peer, 0);
            peer = NULL;
        }

        enet_host_destroy(host);
        enet_deinitialize();
        log_msg("net", "Network deinitialized");
    }
    exitAgeia();
    if (world != NULL)
    {
        delete world;
    }
}

bool GameLabyrinth::createWorld()
{
    std::string dirs[] = {"x", "y"};

	for (int i = 0; i < 2; i++)
	{
		const std::string& dir = dirs[i];
		length[i] = conf->getf("labyrinth.length_" + dir, 1.0f);
		count[i] = conf->geti("labyrinth.count_" + dir, 8);
		Config* loadedWallConfig = conf->find("labyrinth.scene_" + dir);
		if (loadedWallConfig == NULL)
		{
			abort_init("error game res", "Cannot find scene_" + dir + " config");
			return false;
		}
		if (loadedWallConfig->getType() != CONFIG_VALUE_ARRAY)
		{
			abort_init("error game res", "scene_" + dir + " is not array");
		}

		wscene[i] = static_cast<ConfigArray*>(loadedWallConfig);
	}

	labyrinth = generateLabyrinth(count[0], count[1]);

	for (int i = -1; i < labyrinth.getMaxX(); i++)
	{
		for (int j = -1; j < labyrinth.getMaxY(); j++)
		{
			bool right = labyrinth.isRightBorder(i, j);

			if (right && i < labyrinth.getMaxX() && j >= 0)
			{
				Config* currentWallConfig = wscene[0]->getArrayElement(irand(wscene[0]->size()));

				currentWallConfig->setValued("origin[0]", (i + 0.5f)*length[0]);
				currentWallConfig->setValued("origin[1]", j*length[1]);
				GameObject* wall = createGameObject(currentWallConfig);

				world->addObject(wall);
			}

			bool down = labyrinth.isDownBorder(i, j);
			if (down && j < labyrinth.getMaxY() && i >= 0)
			{
				Config* currentWallConfig = wscene[1]->getArrayElement(irand(wscene[1]->size()));

				currentWallConfig->setValued("origin[0]", i*length[0]);
				currentWallConfig->setValued("origin[1]", (j + 0.5f)*length[1]);
				GameObject* wall = createGameObject(currentWallConfig);

                world->addObject(wall);
			}
		}
	}

    return true;
}

bool GameLabyrinth::createCharacter()
{
    CharacterCollector characterCollector;
    world->traverse(characterCollector, ObjectPosition::getIdentity());

    if (characterCollector.characters.empty())
    {
        abort_init("labyrinth", "No character was found");
    }

    characters = characterCollector.characters;
    character = characters[0];
    return true;
}

void GameLabyrinth::process()
{
	GameFreeScene::process();

    if (gameState != GAME_WIN && character != NULL)
    {
        v3 char_pos = character->getPosition().getTranslation();
        if ((char_pos - v3(length[0]*(count[0]-1), length[1]*(count[1]-1), 0)).getSquaredLength() < sqr(length[0]/3) + sqr(length[1]/3))
        {
            gameState = GAME_WIN;
        }
    }

	if (timeInfo.frameLength > EPSILON)
	{
		pScene->simulate(timeInfo.frameLength);
		pScene->flushStream();
		pScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	}

    if (net_role != NET_SINGLE)
    {
        processNetwork();
    }
}

void GameLabyrinth::processNetwork()
{
    ENetEvent event;
    
    while (enet_host_service(host, &event, 0) > 0)
    {
        switch (event.type)
        {
        case ENET_EVENT_TYPE_CONNECT:
            log_msg("net", "A new client connected from " 
                + IntToStr(event.peer->address.host)
                + ":" + IntToStr(event.peer->address.port));
            if (peer == NULL)
            {
                peer = event.peer;

                event.peer->data = "accepted";

                Config* worldConfig = world->getConfig();
                std::string world_config_serialized = worldConfig->Dump();

                NetworkPacket packet;
                packet.kind = NetworkPacket::P_WORLD;

                size_t first_part_size = sizeof(NetworkPacket::PacketKind);
                ENetPacket* enet_packet = enet_packet_create(&packet,
                                                             first_part_size,
                                                             ENET_PACKET_FLAG_RELIABLE);

                size_t packet_size = world_config_serialized.size() + 1 + first_part_size;
                enet_packet_resize(enet_packet, packet_size);

                strcpy((char*)&enet_packet->data[first_part_size], world_config_serialized.c_str());

                enet_peer_send(peer, 0, enet_packet);
                enet_host_flush(host);
                log_msg("net", "sending world to " + IntToStr(peer->address.host) + ":" + IntToStr(peer->address.port));
            }
            else // second connection - dropping
            {
                event.peer->data = "dropped";
                enet_peer_disconnect(peer, 0);
                log_msg("net", "ignoring " + IntToStr(peer->address.host) + ":" + IntToStr(peer->address.port));
            }

            break;

        case ENET_EVENT_TYPE_RECEIVE:
        {
            NetworkPacket packet = *(NetworkPacket*)(event.packet->data);
            switch (packet.kind)
            {
                case NetworkPacket::P_CHARACTER_UPDATE:
                    if (character != NULL)
                    {
                        character->setPosition(packet.data.character_update.position);
                        character->setVelocity(packet.data.character_update.linear_velocity);
                        character->setMomentum(packet.data.character_update.linear_momentum);
                        netTimer.incframe();
                    }
                    break;

                case NetworkPacket::P_WORLD:
                {
                    log_msg("net", "received world from " + IntToStr(peer->address.host) + ":" + IntToStr(peer->address.port));

                    size_t first_part_size = sizeof(NetworkPacket::PacketKind);
                    char* str = (char*)malloc(event.packet->dataLength - first_part_size);
                    memcpy(str, (char*)(event.packet->data) + first_part_size, event.packet->dataLength - first_part_size);
                    Config* new_world = parseConfig(str);
                    free(str);
                    world = static_cast<Combiner*>(createGameObject(new_world));
                    createCharacter();
                    if (graphEngine != NULL)
                    {
                        graphEngine->clear();
                        graphEngine->inject(world);
                    }
                    createPhysicWorld();
                    netTimer.start();
                    log_msg("net", "World from " + IntToStr(peer->address.host) + ":" + IntToStr(peer->address.port) + " created");
                    break;
                }
            }
            enet_packet_destroy(event.packet);
            
            break;
        }
           
        case ENET_EVENT_TYPE_DISCONNECT:
            if (event.peer->data != NULL && std::string(static_cast<char*>(event.peer->data)) == "accepted")
            {
                log_msg("net", "Client " + IntToStr(event.peer->address.host) + ":" + IntToStr(event.peer->address.port) + " disconnecting");
                peer = NULL;
            }
            else
            {
                log_msg("net", "Client " + IntToStr(event.peer->address.host) + ":" + IntToStr(event.peer->address.port) + " dropped");
            }
            event.peer->data = NULL;
        }
    }
    if (net_role == NET_SERVER && peer != NULL)
    {
        sendInformationToClients();
    }
}

void GameLabyrinth::sendInformationToClients()
{
    if (netTimer.lap() > 1.0/30.0) // max 30 fps
    {
        NetworkPacket packet;
        packet.kind = NetworkPacket::P_CHARACTER_UPDATE;
        packet.data.character_update.position =  character->getPosition();
        packet.data.character_update.linear_velocity = to_simple(character->getVelocity());
        packet.data.character_update.linear_momentum = to_simple(character->getMomentum());

        ENetPacket* enet_packet = 
            enet_packet_create(&packet, 
                               sizeof(packet.data.character_update) + sizeof(packet.kind),
                               0);

        enet_peer_send(peer, 0, enet_packet);
        enet_host_flush(host);

        netTimer.nextlap();
    }
}

void GameLabyrinth::handleEventKeyDown(const std::string& key)
{
	if (key == "return" && character != NULL)
	{
		if (cameraMode == C_FIXED)
		{
			cameraMode = C_FREE;
            character->setInput(NULL);
		}
		else
		{
			cameraMode = C_FIXED;
            character->setInput(input);
		}
	}
    else
    {
        GameFreeScene::handleEventKeyDown(key);
    }
}

void GameLabyrinth::handleMouse(double dx, double dy)
{
    if (cameraMode == C_FIXED)
    {
        v3 dir = spectator.camera.getDirection();
        dir.rotateZ(dx*0.1);
        dir.rotateAxis(-dy*0.1, dir.crossProduct(v3(0,0,1).getNormalized()));
        spectator.camera.setDirection(dir);
    }
    else
    {
        GameFreeScene::handleMouse(dx, dy);
    }
}


std::string GameLabyrinth::getWindowCaption()
{
    std::string str = "Labyrinth FPS " + graphTimer.getfps_s()
			+ " Batches: " + IntToStr(graphEngine->total.batchCount)
			+ " Faces: " + IntToStr(graphEngine->total.triangleCount);

    if (peer != NULL && net_role == NET_CLIENT)
    {
        str += " packets/s: " + netTimer.getfps_s();
    }

    if (gameState == GAME_WIN)
    {
        str += " *** YOU ARE WINNER *** ";
    }

    return str;
}

void GameLabyrinth::draw(GraphEngine& graph)
{
	// Render all actors
	int nbActors = pScene->getNbActors();
	NxActor** actors = pScene->getActors();
	while (nbActors--)
	{
		NxActor* actor = *actors++;
		if (!actor->userData)
		{
			continue;
		}
        character->setPosition(to_matrix34(actor->getGlobalPose()));
        character->setDirection(spectator.camera.getDirection());
	}

    if (cameraMode == C_FIXED && character != NULL)
    {
        float len = 2;
        v3 dir = spectator.camera.getDirection();
        v3 crossingPosition;
        Plane triangle;

        GameObject* crossingObject;
        if (graphEngine->findCollision(Line(character->getPosition().getTranslation(), -dir*len), crossingObject, crossingPosition, triangle))
        {
            spectator.camera.setPosition(character->getPosition().getTranslation()*0.1f + crossingPosition*0.9f);
        }
        else
        {
            spectator.camera.setPosition(character->getPosition().getTranslation() - dir*len);
        }
        spectator.camera.setUpVector(v3(0,0,1));
        info.camera = spectator.camera;
    }
    
	GameFreeScene::draw(graph);
}

bool GameLabyrinth::initAgeia()
{
	// Initialize PhysicsSDK
	NxPhysicsSDKDesc desc;
	NxSDKCreateError errorCode = NXCE_NO_ERROR;
	physicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, NULL, new ErrorStream(), desc, &errorCode);
	if (physicsSDK == NULL) 
	{
		error("ageia", std::string("SDK create error (") + IntToStr(errorCode) + " - " + getNxSDKCreateError(errorCode) + ").");
		return false;
	}
#if SAMPLES_USE_VRD
	// The settings for the VRD host and port are found in SampleCommonCode/SamplesVRDSettings.h
	if (gPhysicsSDK->getFoundationSDK().getRemoteDebugger())
		gPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect(SAMPLES_VRD_HOST, SAMPLES_VRD_PORT, SAMPLES_VRD_EVENTMASK);
#endif

	physicsSDK->setParameter(NX_SKIN_WIDTH, conf->getf("physic.skin_width", 0.00f));

	log_msg("ageia", "Ageia connected");
	return true;
}

bool GameLabyrinth::createPhysicWorld()
{
    if (pScene != NULL)
    {
        physicsSDK->releaseScene(*pScene);
        pScene = NULL;
    }
    	// Create a scene
	NxSceneDesc sceneDesc;
    global_gravity = conf->getv3("physic.global_gravity", v3(0.0f, 0.0f, -9.8f));
	sceneDesc.gravity = tonx(global_gravity);
	pScene = physicsSDK->createScene(sceneDesc);
	if (pScene == NULL) 
	{
		error("ageia", "Unable to create a PhysX scene");
		return false;
	}

	// Set default material
	NxMaterial* defaultMaterial = pScene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(conf->getf("physic.restitution", 0.0f));
	defaultMaterial->setStaticFriction(conf->getf("physic.static_friction", 0.5f));
	defaultMaterial->setDynamicFriction(conf->getf("physic.dynamic_friction", 0.5f));

    AgeiaInjector visitor(*this);
    world->traverse(visitor, ObjectPosition::getIdentity());

	return true;
}

void GameLabyrinth::exitAgeia()
{
	if (physicsSDK != NULL)
	{
		if (pScene != NULL)
		{
			physicsSDK->releaseScene(*pScene);
		}
		pScene = NULL;
		NxReleasePhysicsSDK(physicsSDK);
		physicsSDK = NULL;

		log_msg("ageia", "Ageia exited");
	}
}

NxActor* GameLabyrinth::createSurface(const GraphObject& object, const ObjectPosition& position, bool _static)
{
    NxVec3* fsVerts = NULL;
    NxU32* fsFaces = NULL;
//    NxVec3* fsNormals = NULL;

    // Initialize flat surface verts
    const VertexVector& vertexes = *object.getVertexes();
    size_t nbVerts = vertexes.size();
    fsVerts = new NxVec3[vertexes.size()];

    // Build flat surface
    for (size_t i = 0; i < vertexes.size(); i++)
    {
        fsVerts[i] = tonx(vertexes[i]);
    }

    const TriangleVector& triangles = object.getAllFaces()->triangles;

    // Initialize flat surface faces
    size_t nbFaces = triangles.size();
    fsFaces = new NxU32[nbFaces*3];

    size_t k = 0;
    for (size_t i = 0; i < nbFaces; i++)
    {
        // Create first triangle
        fsFaces[k] = triangles[i].a[0];
        fsFaces[k+1] = triangles[i].a[1];
        fsFaces[k+2] = triangles[i].a[2];
        k+=3;
    }

    // allocate flat surface materials -- one for each face
//    fsMaterials = new NxMaterialIndex[nbFaces];

    // Build vertex normals
//    fsNormals = new NxVec3[nbFaces];
//    NxBuildSmoothNormals(nbFaces, nbVerts, fsVerts, fsFaces, NULL, fsNormals, true);

    NxTriangleMeshDesc fsDesc;
    fsDesc.numVertices = nbVerts;
    fsDesc.numTriangles = nbFaces;
    fsDesc.pointStrideBytes = sizeof(NxVec3);
    fsDesc.triangleStrideBytes = 3*sizeof(NxU32);
    fsDesc.points = fsVerts;
    fsDesc.triangles = fsFaces;       
    fsDesc.flags = 0;

    // Add the mesh material data
//    fsDesc.materialIndexStride = sizeof(NxMaterialIndex);
//    fsDesc.materialIndices = fsMaterials;

    NxTriangleMeshShapeDesc fsShapeDesc;

    NxInitCooking();

    MemoryWriteBuffer buf;
    bool status = NxCookTriangleMesh(fsDesc, buf);
    fsShapeDesc.meshData = physicsSDK->createTriangleMesh(MemoryReadBuffer(buf.data));

    if (fsShapeDesc.meshData)
    {
	    NxBodyDesc bodyDesc;
	    bodyDesc.angularDamping	= 0.5f;

        NxActorDesc actorDesc;
        actorDesc.shapes.pushBack(&fsShapeDesc);
        if (!_static)
        {
            actorDesc.body			= &bodyDesc;
        }
        for (int i = 0; i < 3; i++)
        {
            v3 row = position.getRow(i);
            actorDesc.globalPose.M.setRow(i, NxVec3(row.x, row.y, row.z));
        }
        v3 pos = position.getTranslation();
        actorDesc.globalPose.t  = NxVec3(pos.x, pos.y, pos.z);

    	actorDesc.density		= 10.0f;

        NxActor* actor = pScene->createActor(actorDesc);
        return actor;
    }

    return NULL;
}

NxActor* GameLabyrinth::createBox(const GraphObjectBox& box, const ObjectPosition& position, bool _static)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
    v3 size = box.getSize();
	boxDesc.dimensions.set(size.x/2, size.y/2, size.z/2);
	actorDesc.shapes.pushBack(&boxDesc);

    if (!_static)
    {
    	actorDesc.body = &bodyDesc;
    }

	actorDesc.density = 10;
    actorDesc.globalPose = tonx(position);
	return pScene->createActor(actorDesc);	
}
