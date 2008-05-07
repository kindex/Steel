/*id*********************************************************
	File: objects/tag.cpp
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

#include <steel.h>
#include "tag.h"
#include <res/config/config.h>

Tag::Tag() :
    user_info(NULL)
{}


bool Tag::InitFromConfig(Config& conf)
{
	origin =  conf.getv3("origin");
	name = conf.gets("name");

	return true;
}

Config* Tag::getConfig() const
{
    return NULL; // TODO:
}

struct TagCollector : public Visitor
{
    TagCollector(const std::string& tag_name) : 
        tag_name(tag_name)
    {}

    bool visit(IN OUT GameObject* object)
    {
        Tag* tag = dynamic_cast<Tag*>(object);
        if (tag != NULL && tag->name == tag_name)
        {
            tags.push_back(tag);
        }
        return true;
    }

    std::string tag_name;
    std::vector<Tag*> tags;
};

std::vector<Tag*> findTags(GameObject* scene, const std::string& tag_name)
{
    TagCollector tag_collector(tag_name);
    scene->traverse(tag_collector, ObjectPosition::getIdentity());

    return tag_collector.tags;
}
