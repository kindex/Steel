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

StringVector NetworkPacket::extractStrings(size_t offset, size_t total_size) const
{
    unsigned int string_count = *(unsigned int*)((char*)(this) + offset);
    cassert(sizeof(string_count) == 4);
    offset += sizeof(string_count);
    StringVector result;
    for (unsigned int i = 0; i < string_count; i++)
    {
        unsigned int string_length = *(unsigned int*)((char*)(this) + offset);
        offset += sizeof(string_length);
        char* string_ptr = ((char*)(this) + offset);
        if (offset + string_length > total_size)
        {
            error("net", "Illegal packet");
            return result;
        }
        std::string str(string_ptr, string_length);
        result.push_back(str);
    }

    return result;
}

std::string NetworkPacket::extractString(size_t offset, size_t total_size) const
{
    StringVector strings = NetworkPacket::extractStrings(offset, total_size);
    assert(strings.size() == 1);
    return strings[0];
}
