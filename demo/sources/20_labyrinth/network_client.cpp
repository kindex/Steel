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
        std::string peer_name = to_string(event.peer->address);
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

                    case NetworkPacket::S_GAME_INFO:
                        clientReceiveS_GAME_INFO(packet, event.packet->dataLength);
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
					active_character = NULL;
                }
            }
        }
    }
    clientSendInformationToServer();
    if (server.peer != NULL && (server.lastPingRequest < 0 || server.lastPingRequest + conf->getf("net.ping_interval", 1.0f) <= netTimerSend.total()))
    {
        clientSend_PING();
    }
    if (server.peer == NULL && (server.lastConnectTry < 0 || server.lastConnectTry + conf->getf("net.reconnect_interval", 10.0f) <= netTimerSend.current()))
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
    netTimerSend.start();
    netTimerReceive.start();
    active_character = NULL;

    clientConnectToServer();

    return true;
}

bool GameLabyrinth::clientConnectToServer()
{
    if (server.peer != NULL)
    {
        clientDisconnectFromServer();
    }
    server.lastConnectTry = netTimerSend.current();

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
        abort_init("net", "Connection to " + remote_addr + " (" + to_string(address) + ") failed");
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

    size_t new_packet_size = NetworkPacket::setupNetworkPacketString(packet, packet_size, conf->gets("net.name", "noname01"));

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
    if (graphEngine != NULL)
    {
        graphEngine->clear();
        graphEngine->inject(world);
        for EACH(CharacterVector, characters, it)
        {
            graphEngine->inject(*it);
        }
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
                sizeof(NetworkPacket::Format::S_CharacterUpdate::Common) + 
                info.common.character_count*sizeof(NetworkPacket::Format::S_CharacterUpdate::CharacterPosition));
    net_assert(server.client_state == PLAYING);

    std::set<int> filled;

    for (size_t i = 0; i < info.common.character_count; i++)
    {
        NetworkPacket::Format::S_CharacterUpdate::CharacterPosition& pos = info.character_position[i];
        size_t index = info.character_position[i].characterId;

        Character* current = findCharacter(index);

        if (current != NULL)
        {
            if (current != active_character
             || info.common.refresh
             || !current->position_is_set
             || !current->trustPosition(pos.pos))
            {
                current->setCharacterPosition(pos.pos);
            }
        }
        else
        {
            Character* character = createCharacter();
            character->character_id = index;
            addCharacter(character);
            character->setCharacterPosition(pos.pos);
        }
        filled.insert(index);
    }

    for (size_t i = 0; i < info.common.character_count;)
    {
        if (filled.find(characters[i]->character_id) == filled.end())
        {
            deleteCharacter(characters[i]);
        }
        else
        {
            i++;
        }
    }

    netTimerReceive.incframe();
}

void GameLabyrinth::clientReceiveS_BIND_CHAR(NetworkPacket* packet, size_t dataLength)
{
    log_msg("net", "received S_BIND_CHAR to " + server.getNetwornName());
    net_assert(dataLength ==  sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::S_BindCharacter));

    steel::time server_time = packet->data.s_bind_character.server_physic_time;
    steel::time current_time = physicTimer.current();

    physicTimer.add(server_time-current_time);

    size_t character_index = packet->data.s_bind_character.character_index;

    active_character = findCharacter(character_index);
    if (active_character != NULL)
    {
        active_character->input = input;
    }
}

void GameLabyrinth::clientSend_PING()
{
    size_t packet_size = sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::Ping);
    NetworkPacket* packet = (NetworkPacket*)malloc(packet_size);
    packet->kind = NetworkPacket::PING;
    server.lastPingRequest = packet->data.ping.timestamp = netTimerSend.total();

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
    server.ping = netTimerSend.total() - packet->data.pong.timestamp;
}

void GameLabyrinth::clientSendInformationToServer()
{
    if (active_character != NULL && server.peer != NULL && server.client_state == PLAYING && netTimerSend.lap() > 1.0/1.0)
    {
        size_t packet_size = sizeof(NetworkPacket::PacketKind) + 
                             sizeof(NetworkPacket::Format::S_CharacterUpdate::Common) + 
                             1*sizeof(NetworkPacket::Format::S_CharacterUpdate::CharacterPosition);

        NetworkPacket* packet = (NetworkPacket*)malloc(packet_size);
        packet->kind = NetworkPacket::CHAR_UPDATE;
        packet->data.character_update.common.character_count = 1;
        for (size_t i = 0; i < characters.size(); i++)
        {
            if (characters[i] == active_character)
            {
                NetworkPacket::Format::S_CharacterUpdate::CharacterPosition& pos = packet->data.character_update.character_position[0];

                pos.characterId = active_character->character_id;
                pos.pos.position = characters[i]->getPosition();
                pos.pos.linear_velocity = to_simple(characters[i]->getVelocity());
                pos.pos.linear_momentum = to_simple(characters[i]->getAngularMomentum());

                break;
            }
        }
        ENetPacket* enet_packet = 
            enet_packet_create(packet, 
                               packet_size,
                               0);

        enet_peer_send(server.peer, 0, enet_packet);
        free(packet);
        enet_host_flush(host);

        netTimerSend.incframe();
    }
}

void GameLabyrinth::clientReceiveS_GAME_INFO(NetworkPacket* packet, size_t dataLength)
{
    net_assert(dataLength >= sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::S_GameInfo));
    game_state = packet->data.s_game_info.game_state;

    StringVector player_names = packet->extractStrings(sizeof(NetworkPacket::PacketKind) + sizeof(NetworkPacket::Format::S_GameInfo), dataLength);
    StringVector::iterator name = player_names.begin();
    for EACH(CharacterVector, characters, it)
    {
        if (name != player_names.end())
        {
            (*it)->name = *name;
            name++;
        }
    }

    if (game_state == GAME_END)
    {
        Character* winner = findCharacter(packet->data.s_game_info.winner_character_id);
        if (winner != NULL)
        {
            client_winner = winner->name;
        }
    }
}
