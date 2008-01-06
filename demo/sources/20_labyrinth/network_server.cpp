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
        std::string peer_name = IntToStr(event.peer->address.host) + ":" + IntToStr(event.peer->address.port);
        switch (event.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
            {
                log_msg("net", "A new client connecting from " + peer_name);

                Client* new_client = new Client(event.peer);
                new_client->peer = event.peer;
                new_client->peer->data = new_client;
                new_client->state = Client::CONNECTING;
                clients.push_back(new_client);

                serverSendInit(new_client);
                serverSendWorld(new_client);
                break;
            }

            case ENET_EVENT_TYPE_RECEIVE:
            {
                log_msg("net", "Ignoring packet from " + peer_name + " because we are not a client");
                enet_packet_destroy(event.packet);
                
                break;
            }
               
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                log_msg("net", "Client " + peer_name + " disconnecting");
                for EACH(ClientVector, clients, client)
                {
                    if ((*client)->peer == event.peer)
                    {
                        (*client)->disconenct();
                        delete *client;
                        *client = clients.back();
                        clients.pop_back();
                        break;
                    }
                }
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
    StringVector strings;
    strings.push_back(world_config_serialized);

    size_t first_part_size = sizeof(NetworkPacket::PacketKind);
    NetworkPacket* packet = (NetworkPacket*)malloc(first_part_size);
    packet->kind = NetworkPacket::S_WORLD;
    size_t new_packed_size = setupNetworkPacketStrings(packet, first_part_size, strings);

    ENetPacket* enet_packet = enet_packet_create(packet,
                                                 new_packed_size,
                                                 ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(client->peer, 0, enet_packet);
    enet_host_flush(host);
    free(packet);
    client->state = Client::LOADING_WORLD;
}

void GameLabyrinth::serverSendInit(Client* client)
{
    log_msg("net", "sending init to " + IntToStr(client->peer->address.host) + ":" + IntToStr(client->peer->address.port));
    size_t packet_size = sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::S_Init);
    NetworkPacket* packet = (NetworkPacket*)malloc(packet_size);
    packet->kind = NetworkPacket::S_INIT;
    packet->data.init.protocol_version = NetworkPacket::PROTOCOL_VERSION;

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
        enet_host_flush(host);

        netTimer.incframe();
    }
}
