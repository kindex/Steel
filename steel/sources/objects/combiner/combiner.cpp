/*id*********************************************************
	File: objects/combiner/combiner.cpp
	Unit: objects
	Part of: Steel engine
	(C) DiVision, 2006
	Authors:
		* KindeX [Andrey Ivanov, kindexz at gmail]
	License:
		Steel Engine License
	Description:
		Компоновщик графических и физических объектов.
		Позволяет в одном объекте (combiner) объединять свойства
		одного физического объекта [и одного графического].
 ************************************************************/

#include "../../steel.h"
#include "combiner.h"
#include "../game_object_factory.h"
#include "../../audio/audio_interface.h"
#include "../../graph/graph_interface.h"
#include "graph_object.h"
#include "audio_object.h"
#include "../transformation/transformation.h"

Combiner::Combiner(): 
	graph(NULL), 
	audio(NULL),
	transformation(NULL),
    polyhedraPhysicSameAsGraph(false),
    positionKind(POSITION_LOCAL),
    position(ObjectPosition::getIdentity()),
    origin(ObjectPosition::getIdentity())
{}

GraphObject* loadGraphObject(Config& conf, const std::string& configVariable)
{
    GraphObject* graph = NULL;
	Config* graphConf = conf.find(configVariable);
	if (graphConf != NULL)
	{
        std::string graphClass = graphConf->gets("class");
		graph = graphObjectFactory(graphClass);
		if (graph != NULL)
		{
			bool ok = graph->InitFromConfig(*graphConf);
			if (!ok)
			{
				delete graph; 
				graph = NULL; 
			}
		}
	}
    return graph;
}

AudioObject* loadAudioObject(Config& conf, const std::string& configVariable)
{
    AudioObject* audio = NULL;
    Config* audioConf = conf.find(configVariable);
	if (audioConf != NULL)
	{
        std::string audioClass = audioConf->gets("class");
		audio = audioObjectFactory(audioClass);
		if(audio != NULL)
		{
			bool ok =  audio->InitFromConfig(*audioConf);
			if(!ok)
			{
				delete audio; 
				audio = NULL; 
			}
		}
	}
    return audio;
}

Transformation* loadTransformation(Config& conf, const std::string& configVariable)
{
    Transformation* transformation = NULL;
    Config* transformationConf = conf.find(configVariable);
	if (transformationConf != NULL)
	{
        std::string transformationClass = transformationConf->gets("class");
		transformation = transformationFactory(transformationClass);
		if (transformation != NULL)
		{
			bool ok = transformation->InitFromConfig(*transformationConf);
			if (!ok)
			{
				delete transformation;
				transformation = NULL;
			}
			else
			{
				TimeInfo info(0.0f, 0.0f);
				transformation->process(info);
			}
		}
	}
    return transformation;
}

bool Combiner::InitFromConfig(Config& conf)
{
    ObjectPosition corigin;
	corigin.loadIdentity();
	corigin.setScale(conf.getv3("scale", v3(1,1,1)));
	corigin.setTranslation(conf.getv3("origin"));

    setOrigin(corigin);

	position.loadIdentity();
	positionKind = conf.gets("position_kind", "local") == "local" ? POSITION_LOCAL : POSITION_GLOBAL;

    polyhedraPhysicSameAsGraph = conf.getb("polyhedraPhysicSameAsGraph");

    graph = loadGraphObject(conf, "graph");
    audio = loadAudioObject(conf, "audio");
    transformation = loadTransformation(conf, "transformation");
    if (transformation != NULL)
    {
	    position = transformation->getPosition();
    }

	ConfigArray* objectsConfig = conf.getArray("objects");
	if (objectsConfig != NULL)
	{
		const ConfigArray& objectsArray = *static_cast<ConfigArray*>(objectsConfig);
		for EACH_CONST(ConfigArray, objectsArray, it)
		{
			GameObject* newObject = createGameObject(*it);
			if (newObject != NULL)
			{
				addObject(newObject);
			}
		}
	}

	return true;
}

void Combiner::setOrigin(const ObjectPosition& _origin)
{
    origin = _origin;
}

Config* Combiner::getConfig() const
{
    ConfigStruct* result = new ConfigStruct;
    result->setValue("class", new ConfigString("combiner"));
    result->setValue("origin", createV3config(origin.getTranslation()));
    result->setValue("scale", createV3config(origin.getScale()));

    if (graph != NULL)
    {
        result->setValue("graph", graph->getConfig());
    }
    if (audio != NULL)
    {
        result->setValue("audio", audio->getConfig());
    }
    if (transformation != NULL)
    {
        result->setValue("transformation", transformation->getConfig());
    }

    if (!objects.empty())
    {
        ConfigArray* objects_config = new ConfigArray;
        for EACH_CONST(pvector<GameObject*>, objects, it)
        {
            objects_config->push((*it)->getConfig());
        }
        result->setValue("objects", objects_config);
    }

    return result;
}


void Combiner::addObject(GameObject* newObject)
{
	objects.push_back(newObject);
}

bool Combiner::supportsInterface(Engine& engine, IN const InterfaceId id) 
{ 
	if (graph != NULL && id == INTERFACE_GRAPH 
     || audio != NULL && id == INTERFACE_AUDIO)
	{
		return true;
	}
	for EACH(pvector<GameObject*>, objects, it)
	{
		if ((*it)->supportsInterface(engine, id))
		{
			return true;
		}
	}
	return false;
}

void Combiner::bindEngine(Engine& engine, IN const InterfaceId id)
{
	engine.setCurrentObject(this, id);
// TODO:
	if (id == INTERFACE_GRAPH && graph != NULL)
	{
		dynamic_cast<GraphInterface*>(&engine)->setPositionKind(positionKind);
		dynamic_cast<GraphInterface*>(&engine)->setPosition(position*origin);
		graph->bindEngine(engine, id);
	}
    else if (id == INTERFACE_AUDIO && audio != NULL)
	{
//		dynamic_cast<AudioInterface*>(engine)->setPositionKind(positionKind);
//		dynamic_cast<AudioInterface*>(engine)->setPosition(position);
		audio->bindEngine(engine, id);
	}

	ChildrenInterface &cEngine = *dynamic_cast<ChildrenInterface*>(&engine);
	cEngine.clearChildren();

	for EACH(pvector<GameObject*>, objects, it)
	{
		if ((*it)->supportsInterface(engine, id)) 
		{
			cEngine.addChild(*it);
		}
	}
}

const ObjectPosition& Combiner::getLocalPosition()
{
    return position;
}

bool Combiner::updateInformation(Engine& engine, IN const InterfaceId id)
{
	switch (id)
    {
        case INTERFACE_GRAPH:
		    dynamic_cast<GraphInterface*>(&engine)->setPosition(position*origin);
		    if (graph != NULL)
		    {
			    return graph->updateInformation(engine, id);
		    }
            break;

        case INTERFACE_AUDIO:
		    dynamic_cast<AudioInterface*>(&engine)->setPosition(position*origin);
		    if (audio != NULL)
		    {
			    return audio->updateInformation(engine, id);
		    }
            break;
	}

	return false;
}

void Combiner::process(IN const ProcessInfo& info)
{
	if (transformation != NULL)
	{
		transformation->process(info.timeInfo);
		position = transformation->getPosition();
	}
	if (graph != NULL)
	{
		graph->process(info);
	}
	if (audio != NULL)
	{
		audio->setPosition((position*origin).getTranslation());
		audio->process(info);
	}
	for EACH(pvector<GameObject*>, objects, it)
	{
		(*it)->process(info);
	}
}

void Combiner::traverse(Visitor& visitor, const ObjectPosition& base_position)
{
    if (visitor.visit(this))
    {
        if (transformation != NULL)
        {
		    position = transformation->getPosition();
        }

        ObjectPosition new_position = base_position * position * origin;
	    for EACH(pvector<GameObject*>, objects, it)
	    {
            (*it)->traverse(visitor, new_position);
	    }
        if (graph != NULL)
        {
            graph->traverse(visitor, new_position);
        }
        if (audio != NULL)
        {
            audio->traverse(visitor, new_position);
        }
        visitor.postvisit(this, new_position);
    }
}
