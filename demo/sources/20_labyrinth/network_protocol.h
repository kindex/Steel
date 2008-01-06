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
#include <common/containers/string_vector.h>
#include <engine/id_generator.h>
#include <math/matrix34.h>
#include <engine/visitor.h>
#include <common/types.h>

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
    } kind;

    StringVector extractStrings(size_t offset, size_t total_size) const;
    std::string extractString(size_t offset, size_t total_size) const;
    union Format
    {
        struct S_CharacterUpdate
        {
            size_t character_count;
            struct CharacterPosition
            {
                size_t         characterId;
                ObjectPosition position;
                v3simple       linear_velocity;
                v3simple       linear_momentum;
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
        } s_bind_character;
    } data;
}; // struct NetworkPacket

#endif
