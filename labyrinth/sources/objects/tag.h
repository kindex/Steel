/*id*********************************************************
	File: objects/tag.h
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2008
	Authors:
		* KindeX [Andrey Ivanov, kindexz@gmail.com, http://wiki.kindex.lv]
	License:
		Steel Engine License
	Description:
		Метка в сцене, например player_start - означает что в этом месте нужно создать класс Character
 ************************************************************/

#ifndef __OBJECTS_TAG_H
#define __OBJECTS_TAG_H

#include <steel.h>
#include <engine/game_object.h>

class Tag: public GameObject
{
public:
    Tag();

    bool updateInformation(IN OUT Engine&, IN const InterfaceId id) { return false; }
    void process(IN const ProcessInfo&) {}
	bool InitFromConfig(IN Config&);
    Config* getConfig() const;
    bool supportsInterface(Engine& engine, IN const InterfaceId id)
	{
		return false;
	}

    std::string name;
    v3 origin;
    void* user_info;
};

typedef std::vector<Tag*> TagVector;

TagVector findTags(GameObject* scene, const std::string& tag_name);

#endif
