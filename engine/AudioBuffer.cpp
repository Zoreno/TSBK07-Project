/**
 * @file	AudioBuffer.cpp
 * @Author	Joakim Bertils
 * @date	2017-02-18
 * @brief	OpenAL static audio buffer abstraction.
 */

#include "AudioBuffer.h"

#include <AL/alc.h>

#include "WaveFile.h"

AudioBuffer::AudioBuffer(const std::string& path)
	: path{ path }
{
	ALenum error;

	// Dummy read
	alGetError();

	alGenBuffers(1, &handle);

	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		throw AudioException("Could not generate buffer.");
	}

	WaveFile file{ path };

	alBufferData(handle, file.getFormat(), file.getData(), file.getSize(), file.getFrequency());

	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		throw AudioException("Could not fill buffer data.");
	}
}

AudioBuffer::~AudioBuffer()
{
	if(alIsBuffer(handle))
	{
		alDeleteBuffers(1, &handle);
	}
}

ALuint AudioBuffer::getHandle() const
{
	return handle;
}

std::string AudioBuffer::getPath() const
{
	return path;
}
