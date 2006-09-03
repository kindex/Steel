/*id*********************************************************
	Unit: Audio Engine
	Part of: Steel Engine
	(C) DiVision, 2004-2006
	Authors:
		* Kuzmi4 [Andrey Movlyaiko, andrey.movlyaiko@gmail.com]
	License:
		Steel Engine License
	File history:
		[2006.09.09 11:34:29] Kuzmi4: File Creation.
    Description:
		Звуковой движок.
 ************************************************************/

#include "audio_engine.h"

Listener::Listener()
{
	/*
	position = { 0.0f, 0.0f, 0.0f };
	velocity = { 0.0f, 0.0f, 0.0f };
	orientation = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };
	*/
	setPosition(0.0f, 0.0f, 0.0f);
	setVelocity(0.0f, 0.0f, 0.0f);
	setOrientation(v3(0.0f, 0.0f, -1.0f), v3(0.0f, 1.0f, 0.0f));
}

/*
Source::Source(Audio *sound)
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
}
*/
//=============================================================

void AudioEngine::setConfig()
{
	config = loadConfig();
}

void AudioEngine::loadSources()
{
	for (unsigned int i = 0; i < config.size(); i++)
	{
		Audio *audio = resAudio.add(config[i].fileName); 
		Source *source = new Source(audio);
		source->fileName = config[i].fileName;
		source->name = config[i].name;
		source->setGain(config[i].gain);
		source->setPitch(config[i].pitch);
		source->type = config[i].type;
		source->load = config[i].load == "loaded" ? 0 : 1;		// ............
		config[i].loop == "looping" ? source->setLooped(true) : source->setLooped(false);
		sources.push_back(*source);
	}
}