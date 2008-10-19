/*id*********************************************************
	Unit: Labyrinth Game
	Part of: DiVision intro
	(C) DiVision, 2007-2008
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Labyrinth network protocol
 ************************************************************/

#ifndef _NETWORK_PROTOCOL_H_
#define _NETWORK_PROTOCOL_H_

#include "../svn.inc"
#include "character.h"
#include <common/containers/string_vector.h>
#include <engine/id_generator.h>
#include <math/matrix34.h>
#include <engine/visitor.h>
#include <common/types.h>
#include <enet/enet.h>

enum GameState
{
    GAME_LOADING,
    GAME_PLAYING,
    GAME_END,
};

struct NetworkPacket
{
    typedef unsigned int ProtocolVersion;
    static const ProtocolVersion PROTOCOL_VERSION = SVN_REVISION;

    enum PacketKind
    {
        S_INIT = 0x1000,
        C_INIT,
        S_WORLD,
        C_WORLD_LOADED,
        PING,
        PONG,
        CHAR_UPDATE,
        S_BIND_CHAR,
        S_GAME_INFO,
    } kind;
    union Format
    {
        struct S_CharacterUpdate
        {
            struct Common
            {
                bool refresh;
                size_t character_count;
            } common;
            struct CharacterPosition
            {
                size_t                       characterId;
                Character::CharacterPosition pos;
#pragma warning (push)
#pragma warning (disable : 4200)
            } character_position[];
#pragma warning (pop)
        } character_update; // P_CHARACTER_UPDATE
        struct S_Init
        {
            ProtocolVersion protocol_version;
        } s_init;
        struct C_Init
        {
            ProtocolVersion protocol_version;
        } c_init;
        struct S_World
        {
            int x;
            int y;
        } s_world;
        struct C_WorldLoaded
        {
        } c_worldLoaded;
        struct Ping
        {
            steel::time timestamp;
        } ping;
        struct Pong
        {
            steel::time timestamp;
        } pong;
        struct S_BindCharacter
        {
            size_t character_index;
            steel::time server_physic_time;
        } s_bind_character;
        struct S_GameInfo
        {
            GameState game_state;
            int winner_character_id;
        } s_game_info;
    } data;

    static size_t setupNetworkPacketStrings(NetworkPacket*& packet, const size_t size, const StringVector& strings);
    static size_t setupNetworkPacketString(NetworkPacket*& packet, const size_t size, const std::string& string);

    StringVector extractStrings(size_t offset, size_t total_size) const;
    std::string extractString(size_t offset, size_t total_size) const;
}; // struct NetworkPacket

std::string to_string(const ENetAddress&);

#endif
