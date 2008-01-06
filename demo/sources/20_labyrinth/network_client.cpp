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
                        log_msg("net", "received S_INIT from " + peer_name);
                        clientReceiveS_INIT(packet, event.packet->dataLength);
                        break;

                    case NetworkPacket::S_WORLD:
                    {
                        log_msg("net", "received S_WORLD from " + peer_name);
                        clientReceiveS_WORLD(packet, event.packet->dataLength);
                        break;
                    }

                    case NetworkPacket::S_CHARACTER_UPDATE:
                        clientReceiveS_CHARACTER_UPDATE(packet, event.packet->dataLength);
                        break;

                    case NetworkPacket::PONG:
                        clientReceive_PONG(packet, event.packet->dataLength);
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
                if (server.peer != NULL)
                {
                    log_msg("net", "Server " + peer_name + " disconnecting");
                    enet_peer_reset(server.peer);
                    server.peer = NULL;
                    server.client_state = DISCONNECTED;
                }
            }
        }
    }
    if (server.lastPingRequest < 0 || server.lastPingRequest + 1.0f < netTimer.total())
    {
        clientSend_PING();
    }
}

void GameLabyrinth::clientDisconnectFromServer()
{
    if (server.peer != NULL)
    {
        log_msg("net", "Disconnecting from server " + server.getNetwornName());
        enet_peer_disconnect_now(server.peer, 0);
        server.peer = NULL;
    }
}

void GameLabyrinth::clientReceiveS_INIT(NetworkPacket* packet, size_t dataLength)
{
    assert(dataLength == sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::S_Init));
    assert(server.client_state == CONNECTING);
    if (NetworkPacket::PROTOCOL_VERSION == packet->data.s_init.protocol_version)
    {
        clientSendC_INIT();
        server.client_state = LOADING_WORLD;
    }
    else
    {
        log_msg("net", "Disconnecting from server because it uses different protocol version. Our: " + IntToStr(NetworkPacket::PROTOCOL_VERSION) + ". Server: " + IntToStr(packet->data.s_init.protocol_version));
        clientDisconnectFromServer();
    }
}

void GameLabyrinth::clientSendC_INIT()
{
    log_msg("net", "sending C_INIT to " + server.getNetwornName());
    size_t packet_size = sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::C_Init);
    NetworkPacket* packet = (NetworkPacket*)malloc(packet_size);
    packet->kind = NetworkPacket::C_INIT;
    packet->data.c_init.protocol_version = NetworkPacket::PROTOCOL_VERSION;

    StringVector strings;
    strings.push_back("Nick");
    size_t new_packet_size = setupNetworkPacketStrings(packet, packet_size, strings);

    ENetPacket* enet_packet = enet_packet_create(packet,
                                                 new_packet_size,
                                                 ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(server.peer, 0, enet_packet);
    enet_host_flush(host);
    free(packet);
}

void GameLabyrinth::clientReceiveS_WORLD(NetworkPacket* packet, size_t dataLength)
{
    net_assert(server.client_state == LOADING_WORLD);
    size_t first_part_size = sizeof(packet->kind);
    std::string world_str = packet->extractString(first_part_size, dataLength);

    Config* new_world = parseConfig(world_str);
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
    server.client_state = PLAYING;
    clientSendC_WORLD_LOADED();
}

void GameLabyrinth::clientSendC_WORLD_LOADED()
{
    log_msg("net", "sending C_WORLD_LOADED to " + server.getNetwornName());
    size_t packet_size = sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::C_WorldLoaded);
    NetworkPacket* packet = (NetworkPacket*)malloc(packet_size);
    packet->kind = NetworkPacket::C_WORLD_LOADED;

    ENetPacket* enet_packet = enet_packet_create(packet,
                                                 packet_size,
                                                 ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(server.peer, 0, enet_packet);
    enet_host_flush(host);
    free(packet);
}

void GameLabyrinth::clientReceiveS_CHARACTER_UPDATE(NetworkPacket* packet, size_t dataLength)
{
    net_assert(dataLength == sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::S_CharacterUpdate));
    net_assert(server.client_state == PLAYING);
    if (character != NULL)
    {
        character->setPosition(packet->data.character_update.position);
        character->setVelocity(packet->data.character_update.linear_velocity);
        character->setMomentum(packet->data.character_update.linear_momentum);
        netTimer.incframe();
    }
}

void GameLabyrinth::clientSend_PING()
{
    size_t packet_size = sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::Ping);
    NetworkPacket* packet = (NetworkPacket*)malloc(packet_size);
    packet->kind = NetworkPacket::PING;
    server.lastPingRequest = packet->data.ping.timestamp = netTimer.total();

    ENetPacket* enet_packet = enet_packet_create(packet,
                                                 packet_size,
                                                 0);

    enet_peer_send(server.peer, 0, enet_packet);
    enet_host_flush(host);
    free(packet);
}

void GameLabyrinth::clientReceive_PONG(NetworkPacket* packet, size_t dataLength)
{
    net_assert(dataLength == sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::Pong));
    server.ping = netTimer.total() - packet->data.pong.timestamp;
}
