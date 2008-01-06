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

                    case NetworkPacket::CHAR_UPDATE:
                        clientReceive_CHAR_UPDATE(packet, event.packet->dataLength);
                        break;

                    case NetworkPacket::PONG:
                        clientReceive_PONG(packet, event.packet->dataLength);
                        break;

                    case NetworkPacket::S_BIND_CHAR:
                        clientReceiveS_BIND_CHAR(packet, event.packet->dataLength);
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
    clientSendInformationToServer();
    if (server.peer != NULL && (server.lastPingRequest < 0 || server.lastPingRequest + conf->getf("net.ping_interval", 1.0f) <= netTimer.total()))
    {
        clientSend_PING();
    }
    if (server.peer == NULL && (server.lastConnectTry < 0 || server.lastConnectTry + conf->getf("net.reconnect_interval", 10.0f) <= netTimer.total()))
    {
        clientConnectToServer();
    }
 }

bool GameLabyrinth::clientInit()
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
    netTimer.start();
    character = NULL;

    clientConnectToServer();

    return true;
}

bool GameLabyrinth::clientConnectToServer()
{
    if (server.peer != NULL)
    {
        clientDisconnectFromServer();
    }
    server.lastConnectTry = netTimer.total();

    ENetAddress address;
    ENetEvent event;

    std::string remote_addr = conf->gets("net.remote_addr", "localhost");
    int connect_timeout = int(conf->getf("net.connect_timeout", 1)*1000);
    enet_address_set_host(&address, remote_addr.c_str());
    address.port = conf->geti("net.port", 2007);

    server.peer = enet_host_connect(host, &address, 2);
    
    if (server.peer == NULL)
    {
       abort_init("net", "No available peers for initiating an ENet connection");
    }
    
    if (enet_host_service(host, &event, connect_timeout) > 0 &&
        event.type == ENET_EVENT_TYPE_CONNECT)
    {
        log_msg("net", "Connection to " + server.getNetwornName() + " succeeded");
        server.client_state = CONNECTING;
        return true;
    }
    else
    {
        enet_peer_reset(server.peer);
        server.peer = NULL; // TODO: delete server
        abort_init("net", "Connection to " + remote_addr + ":" + IntToStr(address.port)+ " failed");
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

    size_t new_packet_size = setupNetworkPacketString(packet, packet_size, conf->gets("net.name", "noname01"));

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

void GameLabyrinth::clientReceive_CHAR_UPDATE(NetworkPacket* packet, size_t dataLength)
{
    NetworkPacket::Format::S_CharacterUpdate& info = packet->data.character_update;
    net_assert(dataLength == 
                sizeof(NetworkPacket::PacketKind) + 
                sizeof(size_t) + 
                info.character_count*sizeof(NetworkPacket::Format::S_CharacterUpdate::CharacterPosition));
    net_assert(server.client_state == PLAYING);

    for (size_t i = 0; i < info.character_count; i++)
    {
        NetworkPacket::Format::S_CharacterUpdate::CharacterPosition& pos = info.character_position[i];
        size_t index = info.character_position[i].characterId;
        net_assert(index < characters.size());
        characters[index]->setPosition(pos.position);
        characters[index]->setVelocity(pos.linear_velocity);
        characters[index]->setMomentum(pos.linear_momentum);
    }
    netTimer.incframe();
}

void GameLabyrinth::clientReceiveS_BIND_CHAR(NetworkPacket* packet, size_t dataLength)
{
    log_msg("net", "received S_BIND_CHAR to " + server.getNetwornName());
    net_assert(dataLength ==  sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::S_BindCharacter));
    size_t character_index = packet->data.s_bind_character.character_index;
    net_assert(character_index < characters.size());
    character = characters[character_index];
    character->input = input;
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

void GameLabyrinth::clientSendInformationToServer()
{
    if (character != NULL && server.peer != NULL)
    {
        size_t packet_size = sizeof(NetworkPacket::PacketKind) + 
                             sizeof(size_t) + 
                             sizeof(NetworkPacket::Format::S_CharacterUpdate::CharacterPosition);

        NetworkPacket* packet = (NetworkPacket*)malloc(packet_size);
        packet->kind = NetworkPacket::CHAR_UPDATE;
        packet->data.character_update.character_count = 1;
        for (size_t i = 0; i < characters.size(); i++)
        {
            if (characters[i] == character)
            {
                NetworkPacket::Format::S_CharacterUpdate::CharacterPosition& pos = packet->data.character_update.character_position[0];

                pos.characterId = i;
                pos.position = characters[i]->getPosition();
                pos.linear_velocity = to_simple(characters[i]->getVelocity());
                pos.linear_momentum = to_simple(characters[i]->getMomentum());
            }
        }
        ENetPacket* enet_packet = 
            enet_packet_create(packet, 
                               packet_size,
                               0);

        enet_peer_send(server.peer, 0, enet_packet);
        free(packet);
        enet_host_flush(host);
    }
}
