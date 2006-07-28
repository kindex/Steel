#include "openal_engine.h"

ALCdevice *pDevice;
ALCcontext *pContext;
EAXSet m_EAXSet;
EAXGet m_EAXGet;


ALboolean CheckALCError()
{
	ALenum errCode;
//	string err = "ALC error: ";
	if ((errCode = alcGetError(pDevice)) != ALC_NO_ERROR)
	{
//		err += (char *)alcGetString(pDevice, errCode);
		//ERRMSG(err.data());	
		return AL_FALSE;
	}
	return AL_TRUE;
}

ALboolean CheckALError()
{
	ALenum ErrCode;
//	string Err = "OpenAL error: ";
	if ((ErrCode = alGetError()) != AL_NO_ERROR)
	{
//		Err += (char *)alGetString(ErrCode);
//		ERRMSG(Err.data());
		return AL_FALSE;
	}
	return AL_TRUE;
}


bool initializeOpenAL()
{
	ALfloat listenerPosition[] = { 0.0f, 0.0f, 0.0f };
	ALfloat listenerVelocity[] = { 0.0f, 0.0f, 0.0f };
	ALfloat listenerOrientation[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

	// open default sound device
	pDevice = alcOpenDevice(NULL);
	// check for errors
	if (!pDevice)
	{
		//ERRMSG("Default sound device not present!");
		return false;
	}

	// creating rendering context
	pContext = alcCreateContext(pDevice, NULL);
	if (!CheckALCError()) return false;

	alcMakeContextCurrent(pContext);

	// set listener properties
	alListenerfv(AL_POSITION, listenerPosition);
	alListenerfv(AL_VELOCITY, listenerVelocity);
	alListenerfv(AL_ORIENTATION, listenerOrientation);
/*
	// check for EAX 2.0 support
	if ( !alIsExtensionPresent((ALubyte*)"EAX2.0") )
	{
//		std::cout << "\nNo EAX found!\n";
        return FALSE; 
	}

	m_EAXSet = (EAXSet)alGetProcAddress((ALubyte*)"EAXSet");
    m_EAXGet = (EAXGet)alGetProcAddress((ALubyte*)"EAXGet");

	if ( !m_EAXSet || !m_EAXGet )
	{
		std::cout << "\nEAX problems.\n";
        return FALSE;
	}
*/
	return true;
}

void destroyOpenAL()
{
	// clear all buffers and sources
	for (TBuf::iterator i = buffers.begin(); i != buffers.end(); i++)
		alDeleteBuffers(1, &i->second.ID);
	// shut down context
	alcMakeContextCurrent(NULL);
	// destroy context
	alcDestroyContext(pContext);
	// close sound device
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
		std::cout << "\nCannot set environment.\n";
	}
}