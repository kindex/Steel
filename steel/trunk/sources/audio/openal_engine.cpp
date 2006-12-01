/*id*********************************************************
	File: audio/openal_engine.cpp
	Unit: OpenAL Engine
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	Description:
		Звуковой движок.
 ************************************************************/

#include "../steel.h"

#ifdef LIB_OPENAL

#include "openal_engine.h"
#include "../common/logger.h"
#include "../engine/game_object.h"


ALCdevice *pDevice;
ALCcontext *pContext;
EAXSet m_EAXSet;
EAXGet m_EAXGet;

//========================================================================

ALboolean OpenALEngine::CheckALCError()
{
	ALenum errCode;
	string err = "ALC error: ";		
	if ((errCode = alcGetError(pDevice)) != ALC_NO_ERROR)
	{
		err += (char *)alcGetString(pDevice, errCode);
		log_msg("openal audio", err.data());
		return AL_FALSE;
	}
	return AL_TRUE;
}

ALboolean OpenALEngine::CheckALError()
{
	ALenum errCode;
	string err = "OpenAL error: ";
	if ((errCode = alGetError()) != AL_NO_ERROR)
	{
		err += (char *)alGetString(errCode);
		log_msg("openal audio", err.data());
		return AL_FALSE;
	}
	return AL_TRUE;
}


bool OpenALEngine::init(Config* _conf)
{
	conf = _conf;
	if(conf == NULL)
	{
		log_msg("openal init error", "config not found");
		return false;
	}
	log_msg("openal init", "Initializing OpenAL...");

	// open default sound device
	log_msg("openal init", "Opening default sound device...");
	pDevice = alcOpenDevice(NULL);
	// check for errors
	if (!pDevice)
	{
		log_msg("openal init", "ERROR: Default sound device not present!");
		return false;
	}
	log_msg("openal init", "OK  Default sound device is present!");

	// creating rendering context
	log_msg("openal init", "Creating rendering context...");
	pContext = alcCreateContext(pDevice, NULL);
	if (!CheckALCError()) 
	{
		log_msg("openal init", "ERROR: Cannot create rendering context!");
		return false;
	}
	log_msg("openal init", "OK  Rendering context has been created!");

	alcMakeContextCurrent(pContext);

	log_msg("openal init", "OpenAL has been initialized!");

/**/
	// check for EAX 2.0 support
	log_msg("openal init", "Checking for EAX 2.0 support...");
	if ( !alIsExtensionPresent((/*ALubyte* */ ALchar*)"EAX2.0") )
	{
//		std::cout << "\nNo EAX found!\n";
		log_msg("openal init", "ERROR: No EAX 2.0 found!");
        return false; 
	}

	m_EAXSet = (EAXSet)alGetProcAddress((/*ALubyte* */ ALchar*)"EAXSet");
    m_EAXGet = (EAXGet)alGetProcAddress((/*ALubyte* */ ALchar*)"EAXGet");

	if ( !m_EAXSet || !m_EAXGet )
	{
		log_msg("openal init", "ERROR: EAX internal problems!");
		//std::cout << "\nEAX problems.\n";
        return false;
	}
	log_msg("openal init", "EAX 2.0 has been initialized!");
/**/
	return true;
}

bool OpenALEngine::deinit(void)
{
	// clear all buffers and sources
//	for (TBuf::iterator i = buffers.begin(); i != buffers.end(); i++)
//		alDeleteBuffers(1, &i->second.ID);
	// shut down context
	log_msg("openal deinit", "Shutting down audio cntext...");
	alcMakeContextCurrent(NULL);
	// destroy context
	log_msg("openal deinit", "Destroying audio context...");
	alcDestroyContext(pContext);
	// close sound device
	log_msg("openal deinit", "Closing sound device...");
	alcCloseDevice(pDevice);
	return true;
}

void OpenALEngine::setListenerEnvironment(unsigned long environment)
{
	if ( m_EAXSet )
         m_EAXSet(&DSPROPSETID_EAX_ListenerProperties,
                        DSPROPERTY_EAXLISTENER_ENVIRONMENT,
                        0,
                        &environment,
                        sizeof(unsigned long));
	else
	{
		log_msg("openal audio", "ERROR: Cannot set environment!");
		//std::cout << "\nCannot set environment.\n";
	}
}
  

void OpenALEngine::setListener(const Listener &aListener)
{
	AudioEngine::setListener(aListener);

	alListenerfv(AL_POSITION, listener.getPosition());
	alListenerfv(AL_VELOCITY, listener.getVelocity());
	float orientation[6] = { listener.getOrientationAt().x, listener.getOrientationAt().y, listener.getOrientationAt().z, 
								   listener.getOrientationUp().x, listener.getOrientationUp().y, listener.getOrientationUp().z};
	alListenerfv(AL_ORIENTATION, orientation);
}


bool OpenALEngine::inject(GameObject *object)
{
	if(!object->isSuportingInterface(interfaceId)) return false;

	// если объект не хочет добавляться
	if(!object->beforeInject(interfaceId)) return false;

	// кешируем объект
	if(!makeShadowForObject(object)) return false;
	// список глобальных объектов

	setCurrentObject(object);
	object->bindEngine(interfaceId, this);

	return true;
}


bool OpenALEngine::process(void)
{
	// iterator po vsem objectam - kto 4to ho4et
//	for (int i = 0; i < objects.size(); i++)
	{

	}
	return true;
}

bool OpenALEngine::soundPlay(Sound* sound)
{
	Buffer buffer; //= new Buffer();
	Source source; //= new Source();

	alGenBuffers(1, &buffer.buffer);
	CheckALError();
	alBufferData(buffer.buffer, sound->sound->format, sound->sound->data, sound->sound->size, sound->sound->frequency);
	CheckALError();
//		if (sound->data)
//			free(sound->data);
	alGenSources(1, &source.source);
	CheckALError();
	alSourcei (source.source, AL_BUFFER,    buffer.buffer	);
	CheckALError();
	alSourcefv(source.source, AL_POSITION, sound->position);
	alSourcei(source.source, AL_LOOPING, sound->isLoop);
	CheckALError();
	alSourcePlay(source.source);
	CheckALError();
	return true;
}

bool OpenALEngine::soundUpdate(Sound* sound)
{
	return true;
}

void OpenALEngine::addChild(GameObject* child)
{
	if(currentShadow != NULL)
	{
		addChild(*currentShadow, child);
	}
}

void OpenALEngine::addChild(AudioShadow &shadow, GameObject *child)
{
	uid childUid = child->getId();
	
	svector<uid>::iterator it = shadow.children.find(childUid);

	if(it != currentShadow->children.end()) return ; // child have been added before

	if(!child->beforeInject(this->interfaceId)) return; // shild don't want to be added

	if(!makeShadowForObject(child)) return;

	shadow.children.push_back(childUid);

	setCurrentObject(child);

	child->bindEngine(this->interfaceId, this);

	currentShadow = &shadow;
}

bool OpenALEngine::setCurrentObject(GameObject* object)
{
	uid id = object->getId();
	currentShadow = static_cast<AudioShadow*>(getShadow(id));
	if(currentShadow == NULL)
	{
		currentObject = NULL;
		return false;
	}
	currentObject = object;

	return false;
}


#endif
