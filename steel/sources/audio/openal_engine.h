/*id*********************************************************
	File: audio/audio_engine.h
	Unit: OpenAL Engine
	Part of: Steel Engine
	(C) DiVision, 2004-2007
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Звуковой движок.
 ************************************************************/


#ifndef __OPENAL_ENGINE_H
#define __OPENAL_ENGINE_H

#include "../steel.h"

#ifdef LIB_OPENAL

#include "audio_engine.h"
#include "openal/openal_types.h"


// AUDIO SHADOW ==============================
	struct AudioShadow: public Shadow
	{
		//Sound* sound;
		Buffer buffer;
		Source source;
		AudioShadow(Engine* aEngine): Shadow(aEngine) {}
	};
	typedef std::map<uid, AudioShadow*> AudioShadowMap;
// AUDIO SHADOW ==============================


class OpenALEngine: public AudioEngine
{
public:
	void setListener(const Listener& aListener);
	bool init(Config& _conf);
	void pause();
	void unpause();
	bool deinit();
	bool inject(GameObject& object);
	bool process();

	AudioShadow* getShadowClass(GameObject* object) { return new AudioShadow(this); }

	void addChild(GameObject* child);
	void addChild(AudioShadow& shadow, GameObject* child);

	void deleteChild(GameObject* child){}
	void clearChildren() {}
	void setPosition(ObjectPosition) {}
	void setPositionKind(PositionKind) {}
	bool setCurrentObject(GameObject* object);
	bool isSupportingInterface(IN const InterfaceId id)
	{
		return (id & INTERFACE_AUDIO) == id;
	}

	// Configuration/Option Parameters
	bool enabledEAX;
	//float masterVolume;
	

//	Storage* getStorageClass(GameObject *object) { return new Storage(this); }
//	void makeStorageForChildren(GameObject *object) {};


protected:
//#define A(object) ((AudioObject*)(object))
//#define AS(object) ((AudioSound*)(object))

	bool soundPlay(Sound* sound);
	bool soundStop(Sound* sound);
	bool soundPause(Sound* sound);
	bool soundUpdate(Sound* sound);

	void setListenerEnvironment(unsigned long environment);		// extracted from protected ???
	void updateListener(Listener& listener);

	ALboolean CheckALCError();
	ALboolean CheckALError();



	AudioShadow* currentShadow;
	GameObject* currentObject;

	AudioShadowMap shadows;

	// to discuss: vector<Sound*> sounds;	-- "playing" sounds



};

#endif
#endif
