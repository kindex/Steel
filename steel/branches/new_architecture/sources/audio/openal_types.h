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
		OpenAL Types
 ************************************************************/

#ifndef __OPENAL_TYPES_H
#define __OPENAL_TYPES_H

#include "al.h"
#include "alc.h"

class Buffer
{
	ALint frequency;
	ALint size;
	ALint bits;
	ALint channels;
};

class Source
{
	ALfloat		gain;				// Indicate the gain (volume amplification) applied. Range:  ]0.0- ] 
	ALfloat		minGain;			// Indicate minimum source attenuation. Range:  [0.0 - 1.0]
	ALfloat		maxGain;			// Indicate maximum source attenuation. Range:  [0.0 - 1.0]
	ALfloat[3]	position;			// Specify the current location in three dimensional space.
	ALfloat[3]	velocity;			// Specify the current velocity in three dimensional space.
	ALfloat[3]	direction;			// Specify the current direction.
	ALint		sourceRelative;		// AL_TRUE or AL_FALSE
	ALint		sourceState;		// Source state information.
	ALint		sourceType;			// Source type (Static, Streaming or undetermined)
	ALfloat		referenceDistance;	// source specific reference distance. Range:  0.0 - +inf
	ALfloat		maxDistance;		// Indicate distance above which sources are not attenuated using the inverse clamped distance model. Range:  0.0 - +inf
	ALfloat		rolloffFactor;		// source specific rolloff factor. Range:  0.0 - +inf
	ALfloat		coneInnerAngle;		// Directional source, inner cone angle, in degrees. Range:    [0-360] 
	ALfloat		coneOuterAngle;		// Directional source, outer cone angle, in degrees. Range:    [0-360] 
	ALfloat		coneOuterGain;		// Directional source, outer cone gain. Range:    [0.0 - 1.0]
	ALfloat		pitch;				// Specify the pitch to be applied, either at source, or on mixer results, at listener. Range:   [0.5-2.0]
	ALint		isLooop;			// Indicate whether source is looping. Range:   [AL_TRUE, AL_FALSE]
	ALfloat		mSecOffset;			// Source buffer position information
	ALfloat		byteOffset;			// Source buffer position information
	ALfloat		sampleOffset;		// Source buffer position information
};

#endif