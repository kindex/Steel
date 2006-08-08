#include "openal_engine.h"
#include "../common/logger.h"


ALCdevice *pDevice;
ALCcontext *pContext;
EAXSet m_EAXSet;
EAXGet m_EAXGet;

//========================================================================
// classes

AL_Source::AL_Source(Audio * sound)
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


//====================================================================
// extern functions

ALboolean CheckALCError()
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

ALboolean CheckALError()
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


bool initializeOpenAL()
{
	log_msg("openal init", "Initializing OpenAL...");
	ALfloat listenerPosition[] = { 0.0f, 0.0f, 0.0f };
	ALfloat listenerVelocity[] = { 0.0f, 0.0f, 0.0f };
	ALfloat listenerOrientation[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

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

	// set listener properties
	alListenerfv(AL_POSITION, listenerPosition);
	alListenerfv(AL_VELOCITY, listenerVelocity);
	alListenerfv(AL_ORIENTATION, listenerOrientation);

	log_msg("openal init", "OpenAL has been initialized!");

/*
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
/**/
	log_msg("openal init", "EAX 2.0 has been initialized!");
	return true;
}

void destroyOpenAL()
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
}

extern void setListenerEnvironment(unsigned long environment)
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

void soundPlay(AL_Source &sound)
{
	alSourcePlay(sound.source);
}

extern void soundClose(AL_Source &sound)
{
	alSourceStop(sound.source);
	if (alIsSource(sound.source))
		alDeleteSources(1, &sound.source);
}

extern void soundStop(AL_Source &sound)
{
	alSourceStop(sound.source);
}

extern void soundUpdate(AL_Source &sound)
{
	alSourcef (sound.source, AL_PITCH,    sound.getPitch()	);
    alSourcef (sound.source, AL_GAIN,     sound.getGain()		);
    alSourcefv(sound.source, AL_POSITION, sound.getPosition()	);
    alSourcefv(sound.source, AL_VELOCITY, sound.getVelocity()	);
	alSourcei (sound.source, AL_LOOPING,  sound.isLooped()	);
}