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

void GameLabyrinth::serverProcess()
{
    ENetEvent event;
    
    while (enet_host_service(host, &event, 0) > 0)
    {
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                std::string peer_name = IntToStr(event.peer->address.host) + ":" + IntToStr(event.peer->address.port);
                log_msg("net", "A new client connecting from " + peer_name);

                Client* new_client = new Client(event.peer);
                new_client->peer = event.peer;
                new_client->peer->data = new_client;
                new_client->state = CONNECTING;
                clients.push_back(new_client);

                serverSendInit(new_client);
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                NetworkPacket* packet = (NetworkPacket*)(event.packet->data);
                Client* client = (Client*)event.peer->data; // TODO: check if client exists
                std::string peer_name = client->getNetworkName();

                switch (packet->kind)
                {
                    case NetworkPacket::C_INIT:
                        log_msg("net", "received C_INIT from " + peer_name);
                        serverReceiveC_INIT(client, packet, event.packet->dataLength);
                        break;

                    case NetworkPacket::C_WORLD_LOADED:
                        log_msg("net", "received C_WORLD_LOADED from " + peer_name);
                        serverReceiveC_WORLD_LOADED(client, packet, event.packet->dataLength);
                        break;

                    case NetworkPacket::PING:
                        serverReceive_PING(client, packet, event.packet->dataLength);
                        break;

                    default:
                        log_msg("net", "Ignoring packet with kind " + IntToStr(packet->kind) + "  from " + peer_name);
                        break;
                }
                enet_packet_destroy(event.packet);
                
                break;
            }
               
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                Client* client = (Client*)event.peer->data; // TODO: check if client exists
                std::string peer_name = client->getNetworkName();
                log_msg("net", "Client " + peer_name + " disconnecting");
                serverDisconnectClient(client);
            }
        }
    }
    serverSendInformationToClients();
}

void GameLabyrinth::serverSendWorld(Client* client)
{
    log_msg("net", "sending world to " + IntToStr(client->peer->address.host) + ":" + IntToStr(client->peer->address.port));
    Config* worldConfig = world->getConfig();
    std::string world_config_serialized = worldConfig->Dump();

    size_t first_part_size = sizeof(NetworkPacket::PacketKind);
    NetworkPacket* packet = (NetworkPacket*)malloc(first_part_size);
    packet->kind = NetworkPacket::S_WORLD;
    size_t new_packed_size = setupNetworkPacketString(packet, first_part_size, world_config_serialized);

    ENetPacket* enet_packet = enet_packet_create(packet,
                                                 new_packed_size,
                                                 ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(client->peer, 0, enet_packet);
    enet_host_flush(host);
    free(packet);
}

void GameLabyrinth::serverSendInit(Client* client)
{
    log_msg("net", "sending S_INIT to " + IntToStr(client->peer->address.host) + ":" + IntToStr(client->peer->address.port));
    size_t packet_size = sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::S_Init);
    NetworkPacket* packet = (NetworkPacket*)malloc(packet_size);
    packet->kind = NetworkPacket::S_INIT;
    packet->data.s_init.protocol_version = NetworkPacket::PROTOCOL_VERSION;

    ENetPacket* enet_packet = enet_packet_create(packet,
                                                 packet_size,
                                                 ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(client->peer, 0, enet_packet);
    enet_host_flush(host);
    free(packet);
}

void GameLabyrinth::serverSendInformationToClients()
{
    if (netTimer.lap() > 1.0/30.0 && !clients.empty()) // max 30 fps
    {
        for EACH(ClientVector, clients, client)
        {
            if ((*client)->state == PLAYING)
            {
                NetworkPacket packet;
                packet.kind = NetworkPacket::S_CHARACTER_UPDATE;
                packet.data.character_update.position =  character->getPosition();
                packet.data.character_update.linear_velocity = to_simple(character->getVelocity());
                packet.data.character_update.linear_momentum = to_simple(character->getMomentum());

                ENetPacket* enet_packet = 
                    enet_packet_create(&packet, 
                                       sizeof(packet.data.character_update) + sizeof(packet.kind),
                                       0);

                enet_peer_send((*client)->peer, 0, enet_packet);
            }
        }
        enet_host_flush(host);

        netTimer.incframe();
    }
}

void GameLabyrinth::serverReceiveC_INIT(Client* client, NetworkPacket* packet, size_t dataLength)
{
    size_t packet_size = sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::C_Init);
    assert(dataLength >= packet_size);
    assert(client->state == CONNECTING);
    if (packet->data.c_init.protocol_version == NetworkPacket::PROTOCOL_VERSION)
    {
        client->name = packet->extractString(packet_size, dataLength);
        serverSendWorld(client);
        client->state = LOADING_WORLD;
    }
    else
    {
        log_msg("net", "Disconnecting client because it uses different protocol version. Our: " + IntToStr(NetworkPacket::PROTOCOL_VERSION) + ". Server: " + IntToStr(packet->data.c_init.protocol_version));
        serverDisconnectClient(client);
    }
}

void GameLabyrinth::serverReceiveC_WORLD_LOADED(Client* client, NetworkPacket* packet, size_t dataLength)
{
    size_t packet_size = sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::C_WorldLoaded);
    assert(dataLength >= packet_size);
    client->state = PLAYING;
    log_msg("net", client->getNetworkName() + " is ready for playing");
}

void GameLabyrinth::serverReceive_PING(Client* client, NetworkPacket* packet, size_t dataLength)
{
    size_t packet_size = sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::Ping);
    assert(dataLength == packet_size);

    serverSend_PONG(client, packet->data.ping);
}

void GameLabyrinth::serverSend_PONG(Client* client, const NetworkPacket::Format::Ping& pingRequest)
{
    size_t packet_size = sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::Pong);
    NetworkPacket* packet = (NetworkPacket*)malloc(packet_size);
    packet->kind = NetworkPacket::PONG;
    packet->data.pong.timestamp = pingRequest.timestamp;

    ENetPacket* enet_packet = enet_packet_create(packet,
                                                 packet_size,
                                                 0);

    enet_peer_send(client->peer, 0, enet_packet);
    enet_host_flush(host);
    free(packet);
}

void GameLabyrinth::serverDisconnectClient(Client* client)
{
    for EACH(ClientVector, clients, it)
    {
        if (*it == client)
        {
            log_msg("net", "Client " + client->getNetworkName() + " dropped");
            client->disconenct();
            delete client;
            *it = clients.back();
            clients.pop_back();
            break;
        }
    }
}
