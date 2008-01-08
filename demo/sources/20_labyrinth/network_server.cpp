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

                serverSendS_INIT(new_client);
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

                    case NetworkPacket::CHAR_UPDATE:
                        serverReceive_CHAR_UPDATE(client, packet, event.packet->dataLength);
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

bool GameLabyrinth::serverInit()
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

    return true;
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

void GameLabyrinth::serverSendS_INIT(Client* client)
{
    log_msg("net", "sending S_INIT to " + client->getNetworkName());
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

void GameLabyrinth::serverSendS_BIND_CHAR(Client* client, size_t characterIndex)
{
    log_msg("net", "sending S_BIND_CHAR to " + client->getNetworkName());
    size_t packet_size = sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::S_BindCharacter);
    NetworkPacket* packet = (NetworkPacket*)malloc(packet_size);
    packet->kind = NetworkPacket::S_BIND_CHAR;
    packet->data.s_bind_character.character_index = characterIndex;

    ENetPacket* enet_packet = enet_packet_create(packet,
                                                 packet_size,
                                                 ENET_PACKET_FLAG_RELIABLE);

    enet_peer_send(client->peer, 0, enet_packet);
    enet_host_flush(host);
    free(packet);
}

void GameLabyrinth::serverSendInformationToClients()
{
    if (netTimer.lap() > 1.0/100.0 && !clients.empty()) // max 100 fps
    {
        for EACH(ClientVector, clients, client)
        {
            if ((*client)->state == PLAYING)
            {
                size_t total = characters.size();
                for (size_t i = 0; i < characters.size(); i++)
                {
                    if ((*client)->character == characters[i])
                    {
                        total--;
                        break;
                    }
                }

                size_t packet_size = sizeof(NetworkPacket::PacketKind) + 
                                     sizeof(size_t) + 
                                     total*sizeof(NetworkPacket::Format::S_CharacterUpdate::CharacterPosition);

                NetworkPacket* packet = (NetworkPacket*)malloc(packet_size);
                packet->kind = NetworkPacket::CHAR_UPDATE;
                packet->data.character_update.character_count = total;
                size_t j = 0;
                for (size_t i = 0; i < characters.size(); i++)
                {
                    if ((*client)->character != characters[i])
                    {
                        NetworkPacket::Format::S_CharacterUpdate::CharacterPosition& pos = packet->data.character_update.character_position[j];

                        pos.characterId = i;
                        pos.position = characters[i]->getPosition();
                        pos.linear_velocity = to_simple(characters[i]->getVelocity());
                        pos.linear_momentum = to_simple(characters[i]->getMomentum());
                        j++;
                    }
                }
                ENetPacket* enet_packet = 
                    enet_packet_create(packet, 
                                       packet_size,
                                       0);

                enet_peer_send((*client)->peer, 0, enet_packet);
                free(packet);
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
    for (size_t i = 0; i < characters.size(); i++)
    {
        if (characters[i]->owner == Character::FREE)
        {
            characters[i]->owner = Character::CLIENT;
            characters[i]->clientId = client->clientId;
            client->character = characters[i];

            serverSendS_BIND_CHAR(client, i);
            break;
        }
    }
    serverSendS_GAME_INFO(client);
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
    if (client == winner)
    {
        winner = NULL;
        game_state = GAME_PLAYING;
    }

    for EACH(ClientVector, clients, it)
    {
        if (*it == client)
        {
            log_msg("net", "Client " + client->getNetworkName() + " dropped");
            if (client->character != NULL)
            {
                client->character->owner = Character::FREE;
            }
            client->disconenct();
            delete client;
            *it = clients.back();
            clients.pop_back();
            break;
        }
    }
}

void GameLabyrinth::serverReceive_CHAR_UPDATE(Client* client, NetworkPacket* packet, size_t dataLength)
{
    NetworkPacket::Format::S_CharacterUpdate& info = packet->data.character_update;
    net_assert(dataLength == 
                sizeof(NetworkPacket::PacketKind) + 
                sizeof(size_t) + 
                info.character_count*sizeof(NetworkPacket::Format::S_CharacterUpdate::CharacterPosition));
    net_assert(client->state == PLAYING);

    for (size_t i = 0; i < info.character_count; i++)
    {
        NetworkPacket::Format::S_CharacterUpdate::CharacterPosition& pos = info.character_position[i];
        size_t index = info.character_position[i].characterId;
        characters[index]->setPosition(pos.position);
        characters[index]->setVelocity(pos.linear_velocity);
        characters[index]->setMomentum(pos.linear_momentum);
    }
}

void GameLabyrinth::serverSendS_GAME_INFO(Client* client)
{
    size_t packet_size = sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::S_GameInfo);
    NetworkPacket* packet = (NetworkPacket*)malloc(packet_size);
    packet->kind = NetworkPacket::S_GAME_INFO;
    packet->data.s_game_info.game_state = game_state;
    packet->data.s_game_info.i_am_winner = client == winner;

    if (game_state == GAME_END)
    {
        std::string winner_name;
        if (winner == NULL)
        {
            winner_name = "server";
        }
        else
        {
            winner_name = winner->getNetworkName();
        }
        packet_size = setupNetworkPacketString(packet, packet_size, winner_name);
    }

    ENetPacket* enet_packet = enet_packet_create(packet,
                                                 packet_size,
                                                 0);

    enet_peer_send(client->peer, 0, enet_packet);
    enet_host_flush(host);
    free(packet);
}
