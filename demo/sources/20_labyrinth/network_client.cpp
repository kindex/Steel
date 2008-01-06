/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2007-2008
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Labyrinth Game network protocol
 ************************************************************/

#include <enet/enet.h>
#include "20_labyrinth.h"
#include <common/logger.h>
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


void GameLabyrinth::clientProcess()
{
    ENetEvent event;

    while (enet_host_service(host, &event, 0) > 0)
    {
        std::string peer_name = IntToStr(event.peer->address.host) + ":" + IntToStr(event.peer->address.port);
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
                log_msg("net", "Ignoring incoming connection from " + peer_name);
                enet_peer_reset(event.peer);
                break;

            case ENET_EVENT_TYPE_RECEIVE:
            {
                NetworkPacket* packet = (NetworkPacket*)(event.packet->data);
                switch (packet->kind)
                {
                    case NetworkPacket::S_INIT:
                        log_msg("net", "received init from " + peer_name);
                        clientReceiveS_INIT(packet, event.packet->dataLength);
                        break;

                    case NetworkPacket::S_WORLD:
                    {
                        log_msg("net", "received world from " + peer_name);
                        clientReceiveS_WORLD(packet, event.packet->dataLength);
                        break;
                    }

                    case NetworkPacket::S_CHARACTER_UPDATE:
                        clientReceiveS_CHARACTER_UPDATE(packet, event.packet->dataLength);
                        break;

                    default:
                    {
                        error("net", "Ingoring packet from " + peer_name + " with kind " + IntToStr(packet->kind));
                    }
                }
                enet_packet_destroy(event.packet);
                break;
            }

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                if (server != NULL)
                {
                    log_msg("net", "Server " + peer_name + " disconnecting");
                    enet_peer_reset(server);
                    server = NULL;
                    client_state = Client::DISCONNECTED;
                }
            }
        }
    }
}

void GameLabyrinth::clientDisconnectFromServer()
{
    if (server != NULL)
    {
        log_msg("net", "Disconnecting from server " + IntToStr(server->address.host) + ":" + IntToStr(server->address.port));
        enet_peer_disconnect_now(server, 0);
        server = NULL;
    }
}

void GameLabyrinth::clientReceiveS_INIT(NetworkPacket* packet, size_t dataLength)
{
    assert(dataLength == sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::S_Init));
    assert(client_state == Client::CONNECTING);
    if (NetworkPacket::PROTOCOL_VERSION == packet->data.init.protocol_version)
    {
        client_state = Client::LOADING_WORLD;
    }
    else
    {
        log_msg("net", "Disconnecting from server because it uses different protocol version. Our: " + IntToStr(NetworkPacket::PROTOCOL_VERSION) + ". Server: " + IntToStr(packet->data.init.protocol_version));
        clientDisconnectFromServer();
    }
}

void GameLabyrinth::clientReceiveS_WORLD(NetworkPacket* packet, size_t dataLength)
{
    assert(client_state == Client::LOADING_WORLD);
    size_t first_part_size = sizeof(packet->kind);
    StringVector strings = packet->extractStrings(first_part_size, dataLength);
    assert(strings.size() == 1);

    Config* new_world = parseConfig(strings[0]);
    world = static_cast<Combiner*>(createGameObject(new_world));
    createCharacter();
    if (graphEngine != NULL)
    {
        graphEngine->clear();
        graphEngine->inject(world);
    }
    createPhysicWorld();
    netTimer.start();
    log_msg("net", "World created");
    client_state = Client::PLAYING;
}

void GameLabyrinth::clientReceiveS_CHARACTER_UPDATE(NetworkPacket* packet, size_t dataLength)
{
    assert(dataLength == sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::S_CharacterUpdate));
    assert(client_state == Client::PLAYING);
    if (character != NULL)
    {
        character->setPosition(packet->data.character_update.position);
        character->setVelocity(packet->data.character_update.linear_velocity);
        character->setMomentum(packet->data.character_update.linear_momentum);
        netTimer.incframe();
    }
}
