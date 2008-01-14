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
#include "../main.h"
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
        inChar(0),
        current_char(NULL)
    {}

private:
    bool visit(IN OUT GameObject* object)
    {
        if (inChar == 0)
        {
            current_char = dynamic_cast<Character*>(object);
            if (current_char != NULL)
            {
                inChar++;
            }
        }
        else
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
            if (current_char != NULL)
            {
                actor->userData = current_char;
                current_char->physic_object = actor;
            }
        }
        if (inChar > 0)
        {
            inChar--;
        }
        if (inChar == 0)
        {
            current_char = NULL;
        }
        
    }

    GameLabyrinth& game;
    Character*     current_char;
    int inChar;
};

GameLabyrinth::GameLabyrinth():
    character(NULL),
    game_state(GAME_LOADING),
    host(NULL),
    net_role(NET_SINGLE),
    pScene(NULL),
    physicsSDK(NULL),
    winner(NULL)
{}

bool GameLabyrinth::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}

    else if (conf->getb("labyrinth.randomize", false))
    {
        srand((unsigned int)(globalTimer.timestamp()*1000));
    }
    else
    {
        srand((unsigned int)(conf->geti("labyrinth.rand_seed", 0)));
    }

    if (conf->gets("camera.mode", "fixed") == "fixed")
    {
	    cameraMode = C_FIXED;
        spectator.camera.setDirection(v3(1, 1, -0.3f));
    }
    else
    {
	    cameraMode = C_FREE;
    }

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
        if (!serverInit())
        {
            return false;
        }
        game_state = GAME_PLAYING;
    }
    else if (net_role_str == "client")
    {
        if (!clientInit())
        {
            return false;
        }
    }
    else
    {
		abort_init("net", "Unknown net role '" + net_role_str + "'");
    }
    netTimer.start();

    createPhysicWorld();

    return true;
}

void GameLabyrinth::Client::disconenct()
{
    if (peer != NULL)
    {
        log_msg("net", "Disconnecting client " + getNetworkName());
        enet_peer_disconnect_now(peer, 0);
        state = DISCONNECTED;
        peer->data = NULL;
        peer = NULL;
    }
}

GameLabyrinth::~GameLabyrinth()
{
    if (host != NULL)
    {
        for EACH(ClientVector, clients, client)
        {
            (*client)->disconenct();
            delete *client;
        }
        clients.clear();
        if (server.peer != NULL)
        {
            clientDisconnectFromServer();
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
    log_msg("labyrinth", "Exiting from game");
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

	labyrinth = generateLabyrinth(count[0], count[1], conf->getf("labyrinth.crazy_const", 0.02f));

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
        abort_init("labyrinth", "No characters was found");
    }

    characters = characterCollector.characters;

    if (net_role == NET_SERVER)
    {
        for (size_t i = 0; i < characters.size(); i++)
        {
            if (characters[i]->owner == Character::FREE)
            {
                characters[i]->owner = Character::SERVER;
                character = characters[i];
                if (cameraMode == C_FIXED)
                {
                    character->setInput(input);
                }

                break;
            }
        }
    }

    return true;
}

bool GameLabyrinth::isWinner(Character* characher)
{
    v3 char_pos = characher->getPosition().getTranslation();
    return ((char_pos - v3(length[0]*(count[0]-1), length[1]*(count[1]-1), 0)).getSquaredLength() < sqr(length[0]/3) + sqr(length[1]/3));
}

void GameLabyrinth::process()
{
	GameFreeScene::process();

    if (net_role == NET_SERVER && game_state == GAME_PLAYING)
    {
        if (isWinner(character))
        {
            game_state = GAME_END;
            winner = NULL; // NULL means server

            for EACH(ClientVector, clients, client)
            {
                serverSendS_GAME_INFO(*client);
            }
        }

        if (game_state == GAME_PLAYING)
        {
            for EACH(ClientVector, clients, it)
            {
                if ((*it)->character != NULL && isWinner((*it)->character))
                {
                    game_state = GAME_END;
                    winner = *it;

                    for EACH(ClientVector, clients, client)
                    {
                        serverSendS_GAME_INFO(*client);
                    }

                    break;
                }
            }
        }
    }

	if (timeInfo.frameLength > EPSILON)
	{
		pScene->simulate(timeInfo.frameLength);
		pScene->flushStream();
		pScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
	}

    switch (net_role)
    {
        case NET_SERVER:
            serverProcess();
            break;

        case NET_CLIENT:
            clientProcess();
            break;
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
        if (character != NULL)
        {
            character->setDirection(dir);
        }
    }
    else
    {
        GameFreeScene::handleMouse(dx, dy);
    }
}


std::string GameLabyrinth::getWindowCaption()
{
    std::string str = "Labyrinth v" + IntToStr(SVN_REVISION)
        + " FPS: " + graphTimer.getfps_s()
		+ " Batches: " + IntToStr(graphEngine->total.batchCount)
		+ " Faces: " + IntToStr(graphEngine->total.triangleCount);

    switch (net_role)
    {
    case NET_CLIENT:
        if (server.peer == NULL)
        {
            str += " no server";
        }
        else
        {
            str += " sync/s: " + netTimer.getfps_s();
        }
        switch (server.client_state)
        {
            case LOADING_WORLD: str += " Loading world"; break;
            case CONNECTING:    str += " Connecting";    break;
            case PLAYING:       str += " Playing";       
                                str += " ping: " + FloatToStr(server.ping*1000);
                                break;
            case DISCONNECTED:  str += " Disconencted";  break;
        }
        if (game_state == GAME_END)
        {
            if (client_winner.empty())
            {
                str += " *** YOU ARE THE WINNER *** ";
            }
            else
            {
                str += " *** THE WINNER IS " + client_winner + " *** ";
            }
        }

        break;

    case NET_SERVER:
        str += " clients: " + IntToStr(clients.size());
        str += " sync/s: " + netTimer.getfps_s();

        if (game_state == GAME_END)
        {
            if (winner == NULL)
            {
                str += " *** YOU ARE THE WINNER *** ";
            }
            else
            {
                str += " *** THE WINNER IS " + winner->getNetworkName() + " *** ";
            }
        }

        break;
    }

    return str;
}

void GameLabyrinth::draw(GraphEngine& graph)
{
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

    if (world != NULL)
    {
        AgeiaInjector visitor(*this);
        world->traverse(visitor, ObjectPosition::getIdentity());
    }

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

size_t GameLabyrinth::setupNetworkPacketStrings(NetworkPacket*& packet, const size_t size, const StringVector& strings) const
{
    size_t strings_size = sizeof(unsigned int);
    for EACH_CONST(StringVector, strings, str)
    {
        strings_size += sizeof(unsigned int) + str->size();
    }
    packet = (NetworkPacket*)realloc(packet, size + strings_size);
    char* offset = (char*)packet + size;
    *(unsigned int*)offset = strings.size();
    offset += sizeof(unsigned int);
    for EACH_CONST(StringVector, strings, str)
    {
        *(unsigned int*)offset = str->size();
        offset += sizeof(unsigned int);
        for (size_t i = 0; i < str->size(); i++)
        {
            *offset = str->at(i);
            offset += sizeof(char);
        }
    }

    return size + strings_size;
}

size_t GameLabyrinth::setupNetworkPacketString(NetworkPacket*& packet, const size_t size, const std::string& string) const
{
    StringVector strings;
    strings.push_back(string);
    return setupNetworkPacketStrings(packet, size, strings);
}

std::string GameLabyrinth::Client::getNetworkName() const
{
    return name + "@" + IntToStr(peer->address.host) + ":" + IntToStr(peer->address.port);
}

std::string GameLabyrinth::Server::getNetwornName() const
{
    return IntToStr(peer->address.host) + ":" + IntToStr(peer->address.port);
}
