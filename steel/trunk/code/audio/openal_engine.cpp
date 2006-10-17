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


ALCdevice *pDevice;
ALCcontext *pContext;
EAXSet m_EAXSet;
EAXGet m_EAXGet;

//========================================================================
// classes
/*
void AL_Source::initialize(Audio * sound)
{
	setVelocity(0.0f, 0.0f, 0.0f);
	setPosition(0.0f, 0.0f, 0.0f);
	setGain(1.0f);
	setPitch(1.0f);
	setLooped(false);

	alGenBuffers(1, &buffer);
	alBufferData(buffer, sound->format, sound->data, sound->size, sound->frequency);
	if (sound->data)
		free(sound->data);
	alGenSources(1, &source);

	alSourcei (source, AL_BUFFER,   buffer			);

    alSourcef (source, AL_PITCH,    getPitch()		);
    alSourcef (source, AL_GAIN,     getGain()		);
    alSourcefv(source, AL_POSITION, getPosition()	);
    alSourcefv(source, AL_VELOCITY, getVelocity()	);
	alSourcei (source, AL_LOOPING,  isLooped()		);
};
*/


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


bool OpenALEngine::init(const std::string _conf)
{
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
	if ( !alIsExtensionPresent((ALubyte*)"EAX2.0") )
	{
//		std::cout << "\nNo EAX found!\n";
		log_msg("openal init", "ERROR: No EAX 2.0 found!");
        return false; 
	}

	m_EAXSet = (EAXSet)alGetProcAddress((ALubyte*)"EAXSet");
    m_EAXGet = (EAXGet)alGetProcAddress((ALubyte*)"EAXGet");

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


void OpenALEngine::AudioStorage::soundPlay(void)
{
	alSourcePlay(source);
}


void OpenALEngine::AudioStorage::soundPause(void)
{
	alSourcePause(source);
}


void OpenALEngine::AudioStorage::soundStop(void)
{
	alSourceStop(source);
}


void OpenALEngine::AudioStorage::soundClose(void)
{
	alSourceStop(source);
	if (alIsSource(source))
		alDeleteSources(1, &source);
}


void OpenALEngine::AudioStorage::soundUpdate(void)
{
	if(sound != NULL)
	{
		alSourcef (source, AL_PITCH,    pitch	);
		alSourcef (source, AL_GAIN,     gain	);
		alSourcefv(source, AL_POSITION, position);
	//    alSourcefv(source, AL_VELOCITY, getVelocity()	);
		alSourcei (source, AL_LOOPING,  isLoop	);
	}
}


bool OpenALEngine::inject(AudioObject *object)
{
	// если объект не хочет добавляться
	if(!object->AudioBeforeInject()) 
		return false;
	// кешируем объект
	if(!makeStorageForObject(object)) 
		return false;
	makeStorageForChildren(object);
	// список глобальных объектов
	objects.push_back(object);

	return true;
}


void OpenALEngine::makeStorageForChildren(Object *object)
{

}


void OpenALEngine::AudioStorage::fill(Object *object)
{
	Storage::fill(object);

	cache();

	if(sound != NULL)
	{
		alGenBuffers(1, &buffer);
		alBufferData(buffer, sound->format, sound->data, sound->size, sound->frequency);
//		if (sound->data)
//			free(sound->data);
		alGenSources(1, &source);
		alSourcei (source, AL_BUFFER,    buffer	);
		soundUpdate();
		soundPlay();
	}
}


bool OpenALEngine::AudioStorage::cache(void)
{
	Storage::fill(object);

	sound = A(object)->getSound();
	position = A(object)->getPosition().getTranslation();

	isLoop = A(object)->isLooped();
	gain = A(object)->getGain();
	pitch = A(object)->getPitch();
	
	soundUpdate();
	return true;
}


#endif
