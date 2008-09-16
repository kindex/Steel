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
#include <objects/console.h>
#include <math/plane.h>
#include <graph/material.h>
#include <NxPhysics.h>
#include <NxCooking.h>
#include <Stream.h>
#include "../23_ageia_tech/error_stream.h"
#include "../main.h"
#include "steel_nx.h"
#include <algorithm>

class AgeiaInjector : public Visitor
{
public:
    AgeiaInjector(GameLabyrinth& game) : 
        game(game)
    {}

private:
    void postvisit(IN OUT GameObject* object, const ObjectPosition& base_position)
    {
        game.ageiaInject(object, base_position);
    }

    GameLabyrinth& game;
};

GameLabyrinth::GameLabyrinth():
    active_character(NULL),
    game_state(GAME_LOADING),
    host(NULL),
    net_role(NET_SINGLE),
    ageia_scene(NULL),
    physicsSDK(NULL),
    winner(NULL),
    cameraPenalty(1.0f),
    character_model(NULL),
    floor(NULL)
	//winAudioObject(NULL),
	//lossAudioObject(NULL)
{}

void GameLabyrinth::ageiaInject(GameObject* object, const ObjectPosition& base_position)
{
    if (dynamic_cast<GraphObject*>(object) != NULL)
    {
        if (dynamic_cast<GraphObjectBox*>(object) != NULL)
        {
            ageiaCreateBox(*dynamic_cast<GraphObjectBox*>(object), base_position, true);
        }
        else
        {
            ageiaCreateSurface(*dynamic_cast<GraphObject*>(object), base_position, true);
        }
    }
}

bool GameLabyrinth::init(Config& _conf, Input& _input)
{
	if (!GameFreeScene::init(_conf, _input))
	{
		return false;
	}
// resources
    character_model = loadGraphObject(_conf, "character.model");

    if (conf->getb("labyrinth.randomize", false))
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
		abort_init("PhysX", "Cannot init PhysX");
		return false;
	}
	
// network
    std::string net_role_str = conf->gets("net.role", "server");
    loadScene(conf->find("scene"));
	if (net_role_str == "server")
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

    createPhysicWorld();

    netTimerSend.start();
    netTimerReceive.start();
    netTimerReconnect.start();
    refresh_needed = true;
    cell_visibility_set.clear();
    injected_objects.clear();

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

    while (!characters.empty())
    {
        deleteCharacter(characters.back());
    }

	// deleting audio stuff
	if (audioEngine != NULL)
    {
        for EACH(LabyrinthSoundMap, labyrinthSounds, it)
        {
            if (it != labyrinthSounds.end())
            {
                audioEngine->soundStop(it->second->originalSound);
                delete it->second;
                it->second = NULL;
            }
        }
        
    }

    log_msg("labyrinth", "Exiting from game");
}

bool GameLabyrinth::createLabyrinth()
{
    std::string dirs[] = {"x", "y"};

	for (int i = 0; i < 2; i++) // loading walls from config
	{
		const std::string& dir = dirs[i];
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
    cell_size.set(conf->getf("labyrinth.length_x", 1.0f),
                  conf->getf("labyrinth.length_y", 1.0f));

    objects_cell_map.clear();
    all_objects.clear();

	for (int i = -1; i < labyrinth.getMaxX(); i++) // build walls
	{
		for (int j = -1; j < labyrinth.getMaxY(); j++)
		{
			if (labyrinth.isRightBorder(i, j))
			{
				Config* currentWallConfig = wscene[0]->getArrayElement(irand(wscene[0]->size()));

                v2 pos((i + 1.0f)*cell_size.x, (j + 0.5f)*cell_size.y);
				GameObject* wall = createGameObject(currentWallConfig);
                if (dynamic_cast<Combiner*>(wall) != NULL)
                {
                    dynamic_cast<Combiner*>(wall)->setOrigin(ObjectPosition::createTranslationMatrix(v3(pos.x, pos.y, 0)));
                }
                else
                {
                    error("labyrinth", "wall x is not combiner");
                    assert(false);
                    return false;
                }

                all_objects.insert(wall);
                objects_cell_map[std::make_pair(i, j)].insert(wall);
                objects_cell_map[std::make_pair(i+1, j)].insert(wall);
			}

			if (labyrinth.isUpBorder(i, j))
			{
				Config* currentWallConfig = wscene[1]->getArrayElement(irand(wscene[1]->size()));

                v2 pos((i+0.5f)*cell_size.x, (j + 1.0f)*cell_size.y);
				GameObject* wall = createGameObject(currentWallConfig);

                if (dynamic_cast<Combiner*>(wall) != NULL)
                {
                    dynamic_cast<Combiner*>(wall)->setOrigin(ObjectPosition::createTranslationMatrix(v3(pos.x, pos.y, 0)));
                }
                else
                {
                    error("labyrinth", "wall y is not combiner");
                    assert(false);
                    return false;
                }

                all_objects.insert(wall);
                objects_cell_map[std::make_pair(i, j)].insert(wall);
                objects_cell_map[std::make_pair(i, j+1)].insert(wall);
			}
		}
	}

    floor = new Combiner;

    Config* floorConfig = conf->find("labyrinth.floor");
    GameObject* floor1 = createGameObject(floorConfig);
    GameObject* floor2 = createGameObject(floorConfig);
    GameObject* floor3 = createGameObject(floorConfig);
    Combiner* floorCombiner1 = dynamic_cast<Combiner*>(floor1);
    Combiner* floorCombiner2 = dynamic_cast<Combiner*>(floor2);
    Combiner* floorCombiner3 = dynamic_cast<Combiner*>(floor3);
    if (floorCombiner1 != NULL && floorCombiner2 != NULL && floorCombiner3 != NULL )
    {
        v3 size1 = v3(cell_size.x*(labyrinth.getMaxX()-1), cell_size.y*labyrinth.getMaxY(), 1);
        floorCombiner1->setOrigin(ObjectPosition::createTranslationMatrix(v3(size1.x/2, size1.y/2, -size1.z/2)));

        v3 size2 = v3(cell_size.x, cell_size.y*(labyrinth.getMaxY()-1), 1);
        floorCombiner2->setOrigin(ObjectPosition::createTranslationMatrix(v3(size1.x + size2.x/2, size2.y/2, -size2.z/2)));

        v3 size3 = v3(cell_size.x, cell_size.y, 0.1f);
        floorCombiner3->setOrigin(ObjectPosition::createTranslationMatrix(v3(size1.x + size3.x/2, size2.y + size3.y/2, -size1.z - size3.z/2)));

        GraphObjectBox* floorBox1 = dynamic_cast<GraphObjectBox*>(floorCombiner1->getGraphObject());
        GraphObjectBox* floorBox2 = dynamic_cast<GraphObjectBox*>(floorCombiner2->getGraphObject());
        GraphObjectBox* floorBox3 = dynamic_cast<GraphObjectBox*>(floorCombiner3->getGraphObject());
        if (floorBox1 != NULL && floorBox2 != NULL && floorBox3 != NULL)
        {
            floorBox1->material->textureMatrix.texCoordsScale.x *= size1.x/cell_size.x;
            floorBox1->material->textureMatrix.texCoordsScale.y *= size1.y/cell_size.y;

            floorBox2->material->textureMatrix.texCoordsScale.x *= size2.x/cell_size.x;
            floorBox2->material->textureMatrix.texCoordsScale.y *= size2.y/cell_size.y;

            floorBox1->setSize(size1);
            floorBox2->setSize(size2);
            floorBox3->setSize(size3);
            floor->addObject(floor1);
            floor->addObject(floor2);
            floor->addObject(floor3);

            GraphObjectText* exit = new GraphObjectText;
            GraphText exit_text("EXIT",
                              ObjectPosition::createTranslationMatrix(v3(size1.x + size3.x/2, size2.y + size3.y/2, 2)),
                              POSITION_GLOBAL,
                              v2(0.6f, 0.4f),
                              SPRITE_ALIGN_SCREEN,
                              GraphText::ALIGN_CENTER,
                              resFont.add("/font/arial"),
                              color4f(1, 1, 1));

            exit->text.push_back(exit_text);
            exit_text.position.data.vector.z += 6;
            exit_text.size *= 4;
            exit->text.push_back(exit_text);
            floor->addObject(exit);
        }
        else
        {
            error("labyrinth", "floor.graph is not GraphObjectBox");
            assert(false);
            return false;
        }
    }
    else
    {
        error("labyrinth", "floor is not combiner");
        assert(false);
        return false;
    }

    if (graphEngine != NULL && floor != NULL)
    {
        graphEngine->inject(floor);
    }

    return true;
}

void GameLabyrinth::addCharacter(Character* new_character)
{
    characters.push_back(new_character);

    character_index[new_character->character_id] = new_character;
    if (graphEngine != NULL)
    {
        graphEngine->inject(new_character);
    }

    if (ageia_scene != NULL)
    {
        ageiaCreateCharacter(*new_character);
    }
}

void GameLabyrinth::deleteCharacter(Character* character)
{
    if (character == active_character)
    {
        active_character = NULL;
    }

    for EACH(CharacterVector, characters, it)
    {
        if (*it == character)
        {
            characters.erase(it, it+1);
            break;
        }
    }

    if (graphEngine != NULL)
    {
        graphEngine->remove(character);
    }

    if (ageia_scene != NULL)
    {
        ageiaDeleteCharacter(*character);
    }

    for EACH(TagVector, character_starts, tag)
    {
        if ((*tag)->user_info == character)
        {
            (*tag)->user_info = NULL;
        }
    }
    character_index.erase(character_index.find(character->character_id));
    delete character;
}

Character* GameLabyrinth::createCharacter()
{
    Character* character = new Character();

    Config* graph_config = character_model->getConfig();
    character->graph_object = loadGraphObject(*graph_config, "");
    character->force = conf->getf("character.physic.force", 1.0f);
    character->trust_distance = conf->getf("character.physic.trust_distance", 100.0f);
    Config* smoke = conf->find("character.smoke");
    if (smoke == NULL)
    {
        error("labyrinth", "No character smoke config");
    }
    else
    {
        character->smoke = new ParticleSystem();
        character->smoke->InitFromConfig(*smoke);
    }

	if (character->startSound == NULL)
	{
		character->startSound = loadAudioObject(*conf, "character.sounds.start");
		if (character->startSound == NULL)
		{
			delete character->startSound;
			error("audio", "Cannot create character.sounds.start");
		}
	}
	if (character->idleSound == NULL)
	{
		character->idleSound = loadAudioObject(*conf, "character.sounds.idle");
		if (character->idleSound == NULL)
		{
			delete character->idleSound;
			error("audio", "Cannot create character.sounds.idle");
		}
	}
    if (character->moveSound == NULL)
    {
        character->moveSound = loadAudioObject(*conf, "character.sounds.move");
        if (character->moveSound == NULL)
        {
            delete character->moveSound;
            error("audio", "Cannot create character.sounds.move");
        }
    }
    return character;
}

Character* GameLabyrinth::createCharacterStart()
{
    for EACH(TagVector, character_starts, tag)
    {
        if ((*tag)->user_info == NULL)
        {
            Character* character = createCharacter();

            ObjectPosition pos = ObjectPosition::getIdentity();
            pos.setTranslation((*tag)->origin);
            character->setPosition(pos);

            character->character_id = character_id_generator.genUid();
            addCharacter(character);

            (*tag)->user_info = (void*)character;

            return character;
        }
    }

    return NULL;
}

bool GameLabyrinth::createCharacters()
{
    character_starts = findTags(world, "character_start");

    if (character_starts.empty())
    {
        abort_init("labyrinth", "There is no tag 'character_start'");
    }

    if (net_role == NET_SERVER)
    {
        active_character = createCharacterStart();
        if (active_character != NULL)
        {
            active_character->owner = Character::SERVER;
            active_character->name = conf->gets("net.name", "Server");
            if (cameraMode == C_FIXED)
            {
                active_character->setInput(input);
            }
        }
    }

    return true;
}

bool GameLabyrinth::isWinner(Character* characher)
{
    v3 char_pos = characher->getPosition().getTranslation();
    return char_pos.z < 0;
}

void GameLabyrinth::checkForWinner()
{
    if (net_role == NET_SERVER && game_state == GAME_PLAYING)
    {
        for EACH(CharacterVector, characters, it)
        {
            if (isWinner(*it))
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

void GameLabyrinth::processPhysic()
{
	if (timeInfo.frameLength > EPSILON)
	{
        if (world != NULL)
        {
		    world->process(info);
        }
        
        for EACH(CharacterVector, characters, it)
        {
            (*it)->process(info);
        }

		physicTimer.incframe();
		ageia_scene->simulate(timeInfo.frameLength);
		ageia_scene->flushStream();
		ageia_scene->fetchResults(NX_RIGID_BODY_FINISHED, true);

	}
}


void GameLabyrinth::process()
{
	GameFreeScene::process();

	checkForWinner();

	processAudio();

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
    if (game_chat.empty() && key == "tab")
    {
        active_character->setInput(NULL);
        game_chat.append(" ");
    }
    else if (!game_chat.empty() && key =="tab") 
    {
        console.write(game_chat);
        game_chat.erase();
        active_character->setInput(input);
	}
    else if (!game_chat.empty())
    {
        processChatKey(key);
    }

    else if (key == "f7")
    {
        conf->toggle("labyrinth.visibility_check");
    }
    else if (key == "r" && net_role == NET_SERVER)
    {
        restart();
    }
    else if (key == "return" && active_character != NULL)
	{
		if (cameraMode == C_FIXED)
		{
			cameraMode = C_FREE;
            active_character->setInput(NULL);
		}
		else
		{
			cameraMode = C_FIXED;
            active_character->setInput(input);
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
        if (active_character != NULL)
        {
            active_character->setDirection(dir);
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
		+ " Faces: " + IntToStr(graphEngine->total.faceCount);

    switch (net_role)
    {
    case NET_CLIENT:
        if (server.peer == NULL)
        {
            str += " no server";
        }
        else
        {
            str += " sync/s: " + netTimerReceive.getfps_s();
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
        str += " sync/s: " + netTimerSend.getfps_s();

        if (game_state == GAME_END)
        {
            if (winner == active_character)
            {
                str += " *** YOU ARE THE WINNER *** ";
            }
            else
            {
                str += " *** THE WINNER IS " + winner->name + " *** ";
            }
        }

        break;
    }

    return str;
}

void GameLabyrinth::draw(GraphEngine& graph)
{
    if (cameraMode == C_FIXED && active_character != NULL)
    {
        cameraPenalty += info.timeInfo.frameLength;
        if (cameraPenalty > 1 || cameraPenalty < 0)
        {
            cameraPenalty = 1;
        }

        float len = 2;
        v3 dir = spectator.camera.getDirection();
        v3 crossingPosition;
        Plane triangle;

        GameObject* crossingObject;
        float newCameraPenalty;
        v3 origin = active_character->getPosition().getTranslation();

        std::set<const GameObject*> exclude_objects;
        exclude_objects.insert(active_character->graph_object); // TODO: add all children

        if (graphEngine->findCollision(Line(origin, -dir*len), exclude_objects, crossingObject, crossingPosition, triangle))
        {
            newCameraPenalty = (origin-crossingPosition).getLength()/len*0.95f;
        }
        else
        {
            newCameraPenalty = 1.0f;
            spectator.camera.setPosition(active_character->getPosition().getTranslation() - dir*len);
        }
        if (newCameraPenalty <= cameraPenalty)
        {
            cameraPenalty = newCameraPenalty;
        }
        spectator.camera.setPosition(active_character->getPosition().getTranslation() - dir*len*cameraPenalty);

        spectator.camera.setUpVector(v3(0,0,1));
        info.camera = spectator.camera;
    }

    updateVisibleObjects(graph);
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
		error("PhysX", std::string("SDK create error (") + IntToStr(errorCode) + " - " + getNxSDKCreateError(errorCode) + ").");
		if (errorCode == NXCE_PHYSX_NOT_FOUND)
		{
			MessageBox(NULL, "NVIDIA PhysX drivers was not found. Download it from http://developer.nvidia.com/", "Fatal Error", 0);
		}
		return false;
	}
#if SAMPLES_USE_VRD
	// The settings for the VRD host and port are found in SampleCommonCode/SamplesVRDSettings.h
	if (gPhysicsSDK->getFoundationSDK().getRemoteDebugger())
		gPhysicsSDK->getFoundationSDK().getRemoteDebugger()->connect(SAMPLES_VRD_HOST, SAMPLES_VRD_PORT, SAMPLES_VRD_EVENTMASK);
#endif

	physicsSDK->setParameter(NX_SKIN_WIDTH, conf->getf("physic.skin_width", 0.00f));

	log_msg("PhysX", "PhysX connected");
	return true;
}

bool GameLabyrinth::createPhysicWorld()
{
    if (ageia_scene != NULL)
    {
        physicsSDK->releaseScene(*ageia_scene);
        ageia_scene = NULL;
    }
    	// Create a scene
	NxSceneDesc sceneDesc;
    global_gravity = conf->getv3("physic.global_gravity", v3(0.0f, 0.0f, -9.8f));
	sceneDesc.gravity = tonx(global_gravity);
	ageia_scene = physicsSDK->createScene(sceneDesc);
	if (ageia_scene == NULL) 
	{
		error("PhysX", "Unable to create a PhysX scene");
		return false;
	}

	// Set default material
	NxMaterial* defaultMaterial = ageia_scene->getMaterialFromIndex(0);
	defaultMaterial->setRestitution(conf->getf("physic.restitution", 0.0f));
	defaultMaterial->setStaticFriction(conf->getf("physic.static_friction", 0.5f));
	defaultMaterial->setDynamicFriction(conf->getf("physic.dynamic_friction", 0.5f));

    AgeiaInjector visitor(*this);
    if (world != NULL)
    {
        world->traverse(visitor, ObjectPosition::getIdentity());
    }
    if (floor != NULL)
    {
        floor->traverse(visitor, ObjectPosition::getIdentity());
    }

    for EACH(std::set<GameObject*>, all_objects, it)
    {
        (*it)->traverse(visitor, ObjectPosition::getIdentity());
    }

    for EACH(CharacterVector, characters, it)
    {
        ageiaCreateCharacter(**it);
    }

	return true;
}

void GameLabyrinth::exitAgeia()
{
	if (physicsSDK != NULL)
	{
		if (ageia_scene != NULL)
		{
			physicsSDK->releaseScene(*ageia_scene);
		}
		ageia_scene = NULL;
		NxReleasePhysicsSDK(physicsSDK);
		physicsSDK = NULL;

		log_msg("PhysX", "PhysX exited");
	}
}

NxActor* GameLabyrinth::ageiaCreateSurface(const GraphObject& object, const ObjectPosition& position, bool _static)
{
    NxVec3* fsVerts = NULL;
    NxU32* fsFaces = NULL;
//    NxVec3* fsNormals = NULL;

    // Initialize flat surface verts
    const VertexVector* vertexesp = object.getVertexes();
    if (vertexesp == NULL)
    {
        return NULL;
    }
    const VertexVector& vertexes = *vertexesp;
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

    	actorDesc.density = 10.0f;

        NxActor* actor = ageia_scene->createActor(actorDesc);
        return actor;
    }

    return NULL;
}

NxActor* GameLabyrinth::ageiaCreateBox(const GraphObjectBox& box, const ObjectPosition& position, bool _static)
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
	return ageia_scene->createActor(actorDesc);	
}

void GameLabyrinth::ageiaDeleteCharacter(Character& character)
{
    ageia_scene->releaseActor(*character.physic_object);
}

NxActor* GameLabyrinth::ageiaCreateCharacter(IN OUT Character& character)
{
	// Add a single-shape actor to the scene
	NxActorDesc actorDesc;
	NxBodyDesc bodyDesc;

	// The actor has one shape, a box, 1m on a side
	NxBoxShapeDesc boxDesc;
    v3 size = conf->getv3("character.physic.box_size", v3(1, 1, 1));

	boxDesc.dimensions.set(size.x/2, size.y/2, size.z/2);
	actorDesc.shapes.pushBack(&boxDesc);

	actorDesc.body = &bodyDesc;
    actorDesc.flags = NX_BF_KINEMATIC;

	actorDesc.density = 10;
    actorDesc.globalPose = tonx(character.origin);

    NxActor* actor = ageia_scene->createActor(actorDesc);
    actor->userData = (void*)&character;
    character.physic_object = actor;

	return actor;
}

std::string GameLabyrinth::Client::getNetworkName() const
{
    return name + "@" + to_string(peer->address);
}

std::string GameLabyrinth::Server::getNetwornName() const
{
    return to_string(peer->address);
}

void GameLabyrinth::bind(GraphEngine& engine)
{
    GameFreeScene::bind(engine);
    
    for EACH(CharacterVector, characters, it)
    {
        engine.inject(*it);
    }

    if (floor != NULL)
    {
        engine.inject(floor);
    }

    engine.inject(&console);

    updateVisibleObjects(engine);
}

Character* GameLabyrinth::findCharacter(uid character_id)
{
    std::map<uid, Character*>::iterator it = character_index.find(character_id);
    if (it == character_index.end())
    {
        return NULL;
    }
    else
    {
        return it->second;
    }
}

void GameLabyrinth::processChatKey(std::string chat_key)
{
    if (chat_key == "space") game_chat.append(" ");
    else if (chat_key.length() == 1) game_chat.append(chat_key);
}

void GameLabyrinth::restart()
{
    for EACH(TagVector, character_starts, tag)
    {
        (*tag)->user_info = NULL;
    }

    CharacterVector::iterator it = characters.begin();
    for EACH(TagVector, character_starts, tag)
    {
        if (it != characters.end())
        {
            ObjectPosition pos = ObjectPosition::getIdentity();
            pos.setTranslation((*tag)->origin);
            (*it)->setPosition(pos);
            (*it)->setVelocity(zero);
            (*it)->setAngularMomentum(zero);

			
			if ((*it)->startSound != NULL && audioEngine != NULL)
			{
				(*it)->startSound->setPosition(pos.getVector());
				(*it)->startSound->process(ProcessInfo());
                // TODO: should be check here, if files are not present - fatal error
				audioEngine->soundStop( dynamic_cast<SimpleSound*>((*it)->startSound)->originalSound );
				audioEngine->soundPlay( dynamic_cast<SimpleSound*>((*it)->startSound)->originalSound );
			}

			if ((*it)->idleSound != NULL && audioEngine != NULL)
			{
				(*it)->idleSound->setPosition(pos.getVector());
				(*it)->idleSound->process(ProcessInfo());
				audioEngine->soundStop( dynamic_cast<SimpleSound*>((*it)->idleSound)->originalSound );
				audioEngine->soundPlay( dynamic_cast<SimpleSound*>((*it)->idleSound)->originalSound );
			}

            if ((*it)->moveSound != NULL && audioEngine != NULL)
			{
				(*it)->moveSound->setPosition(pos.getVector());
				(*it)->moveSound->process(ProcessInfo());
				audioEngine->soundStop( dynamic_cast<SimpleSound*>((*it)->moveSound)->originalSound );
   				//audioEngine->soundPlay( dynamic_cast<SimpleSound*>((*it)->moveSound)->originalSound );
			}

            (*tag)->user_info = (void*)*it;
            it++;
        }
        else
        {
            break;
        }
    }
    
    refresh_needed = true;
    winner = NULL;
    game_state = GAME_PLAYING;

	// stop audio stuff - NOT CLEAR!!!
    for EACH(LabyrinthSoundMap, labyrinthSounds, it)
    {
        if (it != labyrinthSounds.end())
        {
            audioEngine->soundStop(it->second->originalSound);
        }
    }
}

void GameLabyrinth::updateVisibleObjects(GraphEngine& engine)
{
    bool visibility_check = conf->getb("labyrinth.visibility_check", true);

    if (visibility_check)
    {
        current_sell_set = calculateCellVisibilitySet(labyrinth,
                                                      info.camera.getPosition(),
//                                                      characters[0]->getPosition().getTranslation(),
                                                      cell_size);

        GameObjectSet now_injected_objects;
        for EACH(CellVisibilitySet, current_sell_set, cell)
        {
            for EACH(GameObjectSet, objects_cell_map[*cell], object)
            {
                now_injected_objects.insert(*object);
                if (injected_objects.find(*object) == injected_objects.end())
                {
                    injected_objects.insert(*object);
                    graphEngine->inject(*object);
                }
            }
        }

        std::vector<GameObject*> a;

        std::set_difference(injected_objects.begin(),
                            injected_objects.end(),
                            now_injected_objects.begin(),
                            now_injected_objects.end(),
                            std::back_inserter(a));

        for EACH(std::vector<GameObject*>, a, it)
        {
            graphEngine->remove(*it);
            injected_objects.erase(injected_objects.find(*it));
        }
    }
    else if (all_objects.size() != injected_objects.size())
    {
        for EACH(std::set<GameObject*>, all_objects, object)
        {
            if (injected_objects.find(*object) == injected_objects.end())
            {
                injected_objects.insert(*object);
                graphEngine->inject(*object);
            }
        }
    }
}

void GameLabyrinth::processAudio()
{
    // Loading all sounds
    for (int id = WIN; id <= SoundCount; id++)
    {
        LabyrinthSoundMap::iterator it = labyrinthSounds.find(static_cast<LabyrinthSoundID>(id));
        if (it == labyrinthSounds.end())
	    {
		    AudioObject* snd = loadAudioObject(*conf, "sounds.win");

		    SimpleSound* sndAudioObject = dynamic_cast<SimpleSound*>(snd);
		    if (sndAudioObject == NULL)
		    {
			    delete snd;
			    error("audio", "Cannot create sounds." + id);
		    }
		    else
		    {
                labyrinthSounds[static_cast<LabyrinthSoundID>(id)] = sndAudioObject;
                //delete sndAudioObject;
                sndAudioObject = NULL;
		    }
	    }
    }

	switch (net_role)
	{
		case NET_CLIENT:
			if (game_state == GAME_END)
			{
				if (client_winner.empty())
				{
					// winner sound playing
                    LabyrinthSoundMap::iterator it = labyrinthSounds.find(WIN);
                    if (it != labyrinthSounds.end())
					{
                        audioEngine->soundPlay(labyrinthSounds[WIN]->originalSound);
					}
				}
				else
				{
					// looser sound playing
                    LabyrinthSoundMap::iterator it = labyrinthSounds.find(LOSS);
					if (it != labyrinthSounds.end())
					{
				        audioEngine->soundPlay(labyrinthSounds[LOSS]->originalSound);
					}
				}
			}
			break;

		case NET_SERVER:
			if (game_state == GAME_END)
			{
				if (winner == active_character)
				{
					// winner sound playing
                    LabyrinthSoundMap::iterator it = labyrinthSounds.find(WIN);
                    if (it != labyrinthSounds.end())
					{
                        audioEngine->soundPlay(labyrinthSounds[WIN]->originalSound);
					}
				}
				else
				{
					// looser sound playing
					LabyrinthSoundMap::iterator it = labyrinthSounds.find(LOSS);
					if (it != labyrinthSounds.end())
					{
				        audioEngine->soundPlay(labyrinthSounds[LOSS]->originalSound);
					}
				}
			}
			break;
	}

	// updating objects
	CharacterVector::iterator it = characters.begin();
    for EACH(TagVector, character_starts, tag)
    {
		if (it != characters.end())
		{

    		if ((*it)->startSound != NULL && audioEngine != NULL)
			{
				(*it)->startSound->setPosition((*it)->getPosition().getVector());
				(*it)->startSound->process(ProcessInfo());
				bool startd = dynamic_cast<SimpleSound*>((*it)->startSound)->isStarted();
				if (startd)
				{
					audioEngine->soundUpdate( dynamic_cast<SimpleSound*>((*it)->startSound)->originalSound );
				}
				else
				{
					dynamic_cast<SimpleSound*>((*it)->startSound)->start();
					audioEngine->soundPlay( dynamic_cast<SimpleSound*>((*it)->startSound)->originalSound );
				}
			}

			if ((*it)->idleSound != NULL && audioEngine != NULL)
			{
				(*it)->idleSound->setPosition((*it)->getPosition().getVector());
				(*it)->idleSound->process(ProcessInfo());
				if (dynamic_cast<SimpleSound*>((*it)->idleSound)->isStarted())
				{
					audioEngine->soundUpdate(dynamic_cast<SimpleSound*>((*it)->idleSound)->originalSound);
				}
				else
				{
					dynamic_cast<SimpleSound*>((*it)->idleSound)->start();
					audioEngine->soundPlay( dynamic_cast<SimpleSound*>((*it)->idleSound)->originalSound );
				}
			

                if ( (dynamic_cast<SimpleSound*>((*it)->idleSound)->isStarted()) && ( (*it)->state == MOVING ))
			    {
                        dynamic_cast<SimpleSound*>((*it)->idleSound)->stop();
				        audioEngine->soundStop( dynamic_cast<SimpleSound*>((*it)->idleSound)->originalSound );
			    }
                if ( (dynamic_cast<SimpleSound*>((*it)->idleSound)->isStarted() == false) && ( (*it)->state == IDLE ))
                {
                    dynamic_cast<SimpleSound*>((*it)->idleSound)->start();
			        audioEngine->soundPlay( dynamic_cast<SimpleSound*>((*it)->idleSound)->originalSound );
                }
                if ( (dynamic_cast<SimpleSound*>((*it)->idleSound)->isStarted()) && ( (*it)->state == IDLE ))
                {
                    audioEngine->soundUpdate(dynamic_cast<SimpleSound*>((*it)->idleSound)->originalSound);
                }
		    }

            if ((*it)->moveSound != NULL && audioEngine != NULL)
			{
                //(*it)->state
				(*it)->moveSound->setPosition((*it)->getPosition().getVector());
				(*it)->moveSound->process(ProcessInfo());
				if ( (dynamic_cast<SimpleSound*>((*it)->moveSound)->isStarted()) && ( (*it)->state == MOVING ))
				{
					audioEngine->soundUpdate(dynamic_cast<SimpleSound*>((*it)->moveSound)->originalSound);
				}
                if ( (dynamic_cast<SimpleSound*>((*it)->moveSound)->isStarted() == false) && ( (*it)->state == MOVING ))
                {
                    dynamic_cast<SimpleSound*>((*it)->moveSound)->start();
				    audioEngine->soundPlay( dynamic_cast<SimpleSound*>((*it)->moveSound)->originalSound );
                }
                if ( (dynamic_cast<SimpleSound*>((*it)->moveSound)->isStarted()) && ( (*it)->state == IDLE ))
                {
                    dynamic_cast<SimpleSound*>((*it)->moveSound)->stop();
				    audioEngine->soundStop( dynamic_cast<SimpleSound*>((*it)->moveSound)->originalSound );
                }
			}
			it++;
		}
		else
		{
			break;
		}
    }


	return;
}
