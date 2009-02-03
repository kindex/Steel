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
#include "network_protocol.h"
#include <common/logger.h>
#include <cassert>

#include "../svn.h"
const NetworkPacket::ProtocolVersion PROTOCOL_VERSION = SVN_REVISION;

/*
uint32 string_count
array*string_count
    uint32              string_length
    char[string_length] string
*/

StringVector NetworkPacket::extractStrings(size_t offset, size_t total_size) const
{
    unsigned int string_count = *(unsigned int*)((char*)(this) + offset);
    cassert(sizeof(string_count) == 4);
    offset += sizeof(string_count);

    StringVector result;
    for (unsigned int i = 0; i < string_count; i++)
    {
        unsigned int string_length = *(unsigned int*)((char*)(this) + offset);
        cassert(sizeof(string_length) == 4);
        offset += sizeof(string_length);

        char* string_ptr = ((char*)(this) + offset);
        if (offset + string_length > total_size)
        {
            error("net", "Illegal packet");
            return result;
        }
        offset += string_length;

        std::string str(string_ptr, string_length);
        result.push_back(str);
    }

    return result;
}

size_t NetworkPacket::setupNetworkPacketStrings(NetworkPacket*& packet, const size_t size, const StringVector& strings)
{
    size_t strings_size = sizeof(unsigned int);
    for EACH_CONST(StringVector, strings, str)
    {
        strings_size += sizeof(unsigned int) + str->size();
    }
    packet = (NetworkPacket*)realloc(packet, size + strings_size);
    char* offset = (char*)packet + size;
    *(unsigned int*)offset = strings.size();
    offset += sizeof(unsigned int);

    for EACH_CONST(StringVector, strings, str)
    {
        *(unsigned int*)offset = str->size();
        offset += sizeof(unsigned int);
        for (size_t i = 0; i < str->size(); i++)
        {
            *offset = str->at(i);
            offset += sizeof(char);
        }
    }

    return size + strings_size;
}

size_t NetworkPacket::setupNetworkPacketString(NetworkPacket*& packet, const size_t size, const std::string& string)
{
    StringVector strings;
    strings.push_back(string);
    return setupNetworkPacketStrings(packet, size, strings);
}

std::string NetworkPacket::extractString(size_t offset, size_t total_size) const
{
    StringVector strings = NetworkPacket::extractStrings(offset, total_size);
    assert(strings.size() == 1);
    return strings[0];
}

std::string to_string(const ENetAddress& address)
{
    std::string ip = 
        IntToStr((address.host)&255) + "." +
        IntToStr((address.host >> 8)&255) + "." +
        IntToStr((address.host >> 16)&255) + "." +
        IntToStr((address.host >> 24)&255);

    return ip + ":" + IntToStr(address.port);
}
