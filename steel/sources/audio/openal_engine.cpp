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
#include "../common/containers/string_dict.h"
#include "../res/audio/audio.h"
#include "sound.h"
#define NOMINMAX
#include <windows.h>

#include "openal/al.h"
#include "openal/alc.h"
//#include "openal/alu.h"
//#include "openal/alut.h"
#include "openal/eax.h"


ALCdevice *pDevice;
ALCcontext *pContext;
EAXSet m_EAXSet;
EAXGet m_EAXGet;

//========================================================================

ALboolean OpenALEngine::CheckALCError()
{
	ALenum errCode;
	std::string err = "ALC error: ";		
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
	std::string err = "OpenAL error: ";
	if ((errCode = alGetError()) != AL_NO_ERROR)
	{
		err += (char *)alGetString(errCode);
		log_msg("openal audio", err.data());
		return AL_FALSE;
	}
	return AL_TRUE;
}


bool OpenALEngine::init(Config& _conf)
{
	conf = &_conf;
	if(conf == NULL)
	{
		log_msg("openal init error", "config not found");
		return false;
	}

	enabled = conf->getb("enabled", true);
	if (!enabled)
	{
		return false;
	}
	enabledEAX = conf->getb("EAX", false);

	log_msg("openal init", "Initializing OpenAL...");

	// open default sound device
	log_msg("openal init", "Opening default sound device...");
	pDevice = alcOpenDevice(NULL);//alcOpenDevice((ALchar*)"DirectSound3D");
	// check for errors
	if (!pDevice)
	{
		log_msg("openal init warning", "Default sound device not present");
		return false;
	}
	log_msg("openal init", "OK  Default sound device is present!");
//	log_msg("openal init", alcGetString((pDevice),ALC_DEVICE_SPECIFIER) );
//	log_msg("openal init", alcGetString((pDevice),ALC_DEFAULT_DEVICE_SPECIFIER) );
//	log_msg("openal init", alcGetString((pDevice),ALC_EXTENSIONS) );
//	log_msg("openal init", alcGetString((pDevice),ALC_CAPTURE_DEVICE_SPECIFIER) );
//	log_msg("openal init", alcGetString((pDevice),ALC_MINOR_VERSION) );

	
	// creating rendering context
	log_msg("openal init", "Creating rendering context...");
	pContext = alcCreateContext(pDevice, NULL);
	if (!CheckALCError()) 
	{
		log_msg("openal init", "ERROR: Cannot create rendering context!");
		return false;
	}
	log_msg("openal init", "Rendering context has been created");

	alcMakeContextCurrent(pContext);

	masterVolume = conf->getf("mastervolume", 1.0f);

	log_msg("openal init", "OpenAL has been initialized");

	/**/
// TESTING
	log_msg("openal init", "Vendor: " + std::string(alGetString(AL_VENDOR)));
	log_msg("openal init", "Version: " + std::string(alGetString(AL_VERSION)));
	log_msg("openal init", "Renderer: " + std::string(alGetString(AL_RENDERER)));
	log_msg("openal init", "Extensions: " + std::string(alGetString(AL_EXTENSIONS)));
/**/
	const ALchar *pCDeviceList = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);
	if (pCDeviceList)
	{
		log_msg("openal init", "Available Capture Devices are:");

		while (*pCDeviceList)
		{
			log_msg("openal init", pCDeviceList);
			pCDeviceList += strlen(pCDeviceList) + 1;
		}
	}
	else
	{
		log_msg("openal init", "no audio capture devices");
	}

	const ALchar* pDeviceList = alcGetString(NULL, ALC_DEVICE_SPECIFIER);
	if (pDeviceList)
	{
		log_msg("openal init", "Available Devices are:");

		while (*pDeviceList)
		{
			log_msg("openal init", pDeviceList);
			pDeviceList += strlen(pDeviceList) + 1;
		}
	}
	else
	{
		log_msg("openal init", "no devices");
	}

	log_msg("openal init", "Default capture device:");
	if (pCDeviceList)
	{
		log_msg("openal init", alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER));
	}
	else
	{
		log_msg("openal init", "no default capture device");
	}

	log_msg("openal init", "Default device:");
	if (pDeviceList)
	{
		log_msg("openal init", alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER));
	}
	else
	{
		log_msg("openal init", "no default device");
	}

	if (enabledEAX)
	{
	// check for EAX 2.0 support
		log_msg("openal init", "Checking for EAX 2.0 support...");
		if (!alIsExtensionPresent((ALchar*)"EAX2.0"))
		{
	//		std::cout << "\nNo EAX found!\n";
			log_msg("openal init warning", "No EAX 2.0 found");
			enabledEAX = 0;
		//	return false; 
		}
	}
	if (enabledEAX)
	{
		m_EAXSet = (EAXSet)alGetProcAddress((ALchar*)"EAXSet");
		m_EAXGet = (EAXGet)alGetProcAddress((ALchar*)"EAXGet");

		if ( !m_EAXSet || !m_EAXGet )
		{
			log_msg("openal init error", "EAX internal problems");
			//std::cout << "\nEAX problems.\n";
			return false;
		}
		log_msg("openal init", "EAX 2.0 has been initialized");
	};

	return true;
}

bool OpenALEngine::deinit()
{
	// clear all buffers and sources
//	for (TBuf::iterator i = buffers.begin(); i != buffers.end(); i++)
//		alDeleteBuffers(1, &i->second.ID);
	
	for EACH(AudioShadowMap, shadows, it)
	{
		alDeleteBuffers(1, &it->second->buffer.buffer);
		alDeleteSources(1, &it->second->source.source);
	}

	// shut down context
	if (enabledEAX)
		setListenerEnvironment(EAX_ENVIRONMENT_GENERIC);
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
	if (enabledEAX)
	{
		if (m_EAXSet)
		{
			 m_EAXSet(&DSPROPSETID_EAX_ListenerProperties,
							DSPROPERTY_EAXLISTENER_ENVIRONMENT,
							0,
							&environment,
							sizeof(unsigned long));
		}
		else
		{
			log_msg("openal audio error", "Cannot set environment");
			//std::cout << "\nCannot set environment.\n";
		}
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


bool OpenALEngine::inject(GameObject& object)
{
    if (!object.supportsInterface(*this, INTERFACE_AUDIO))
    {
        return false;
    }

	// если объект не хочет добавляться
	if (!object.beforeInject(*this, INTERFACE_AUDIO))
    {
        return false;
    }

	// кешируем объект
	if (!makeShadowForObject(&object, INTERFACE_AUDIO))
    {
        return false;
    }
	// список глобальных объектов

	setCurrentObject(&object, INTERFACE_AUDIO);
	object.bindEngine(*this, INTERFACE_AUDIO);

	return true;
}


bool OpenALEngine::process()
{
	// iterator po vsem objectam - kto 4to ho4et
//	for (int i = 0; i < objects.size(); i++)
	//for (AudioShadowMap::iterator it = shadows.begin(); it != shadows.end(); it++)
	{
		//it->second->
	}
	return true;
}

bool OpenALEngine::soundPlay(Sound* sound)
{
	if (sound == NULL || sound->sound == NULL)
	{
		return false;
	}
	/*
	if (!shadows.empty())
	{
		map<uid, AudioShadow*>::iterator it = shadows.find(sound->id);
		if (it->second->buffer.buffer != 0)
		{
			alSourceStop(it->second->source.source);
			alSourcePlay(it->second->source.source);
		}
		return true;
	}
	*/

	if (sound->isStarted)
	{
		return false;
	}

	AudioShadowMap::iterator it = shadows.find(sound->id);
	if (it != shadows.end())
	{
		soundUpdate(sound);
		//alSourceStop(it->second->source.source);
		alSourcePlay(it->second->source.source);
	}
	else
	{
		AudioShadow* shadow = new AudioShadow(this);
		//buffer = new Buffer();
		//source = new Source();

		alGenBuffers(1, &shadow->buffer.buffer);
		CheckALError();
		alBufferData(shadow->buffer.buffer, sound->sound->format, sound->sound->data, sound->sound->size, sound->sound->frequency);
		//alBufferi(shadow->buffer.buffer, AL_FREQUENCY, 22050);
		CheckALError();
	//		if (sound->data)
	//			free(sound->data);
		alGenSources(1, &shadow->source.source);
		CheckALError();
		alSourcei (shadow->source.source, AL_BUFFER,    shadow->buffer.buffer	);
		CheckALError();
		shadows[sound->id] = shadow;

		// update or set parameters
		//alSourcefv(shadow->source.source, AL_POSITION, sound->position);
		//alSourcei(shadow->source.source, AL_LOOPING, sound->isLoop);
		soundUpdate(sound);
		//soundStop(sound);
		CheckALError();
		
		alSourcePlay(shadow->source.source);
		CheckALError();
	}
	sound->isStarted = true;
	return true;
}

bool OpenALEngine::soundStop(Sound* sound)
{
	AudioShadowMap::iterator it = shadows.find(sound->id);
    if (it != shadows.end())
    {
	    alSourceStop(it->second->source.source);
	    sound->isStarted = false;
	    return true;
    }
    else
    {
       return false;
    }
}

bool OpenALEngine::soundPause(Sound* sound)
{
	AudioShadowMap::iterator it = shadows.find(sound->id);
    if (it != shadows.end())
    {
    	alSourcePause(it->second->source.source);
	    return true;
    }
    else
    {
        return false;
    }
}

bool OpenALEngine::soundUnPause(Sound* sound)
{
	AudioShadowMap::iterator it = shadows.find(sound->id);
	alSourcePlay(it->second->source.source);
	return true;
}

bool OpenALEngine::soundUpdate(Sound* sound)
{
	AudioShadowMap::iterator it = shadows.find(sound->id);
	if (it == shadows.end())
	{
		return false;
	}
	alSourcefv(it->second->source.source, AL_POSITION, sound->position);
	alSourcei(it->second->source.source, AL_LOOPING, sound->isLoop);
	alSourcef(it->second->source.source, AL_GAIN, sound->gain);
	alSourcef(it->second->source.source, AL_PITCH, sound->pitch);
	alSourcef(it->second->source.source, AL_ROLLOFF_FACTOR, sound->rolloffFactor);
	alSourcei(it->second->source.source, AL_SOURCE_RELATIVE, sound->sourceRelative);
/*
	alSourcef(it->second->source.source, AL_REFERENCE_DISTANCE, 5 );
	alSourcef( it->second->source.source, AL_MAX_DISTANCE, 10 );
	//alSourcefv( it->second->source.source, AL_DIRECTION, direction );
    alSourcef( it->second->source.source, AL_CONE_INNER_ANGLE, 18 );
    alSourcef( it->second->source.source, AL_CONE_OUTER_ANGLE, 18 );
    alSourcef( it->second->source.source, AL_CONE_OUTER_GAIN, 0.5f);
*/
	return true;
}

void OpenALEngine::addChild(GameObject* child)
{
	if (currentShadow != NULL)
	{
		addChild(*currentShadow, child);
	}
}

void OpenALEngine::addChild(AudioShadow& shadow, GameObject* child)
{
	uid childUid = child->getObjectId();
	pvector<uid>::const_iterator it = find(shadow.children, childUid);
	if (it != currentShadow->children.end())
    {
        return ; // child have been added before
    }
	if (!child->beforeInject(*this, INTERFACE_AUDIO))
    {
        return; // shild don't want to be added
    }
	if (!makeShadowForObject(child, INTERFACE_AUDIO))
    {
        return;
    }
	shadow.children.push_back(childUid);
	setCurrentObject(child, INTERFACE_AUDIO);
	child->bindEngine(*this, INTERFACE_AUDIO);
	currentShadow = &shadow;
}

bool OpenALEngine::setCurrentObject(GameObject* object, const InterfaceId)
{
	uid id = object->getObjectId();
    currentShadow = static_cast<AudioShadow*>(getShadow(id, INTERFACE_AUDIO));
	if (currentShadow == NULL)
	{
		currentObject = NULL;
		return false;
	}
	currentObject = object;

	return false;
}

void OpenALEngine::pause() // engine pause
{
	paused = true;

	for EACH(AudioShadowMap, shadows, it)
	{
        alSourcePause(it->second->source.source);
	}
}

void OpenALEngine::unpause() // engine unpause
{
	paused = false;

	for EACH(AudioShadowMap, shadows, it)
	{
        alSourcePlay(it->second->source.source);
	}
}

#endif
