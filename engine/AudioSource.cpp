/**
 * @file	AudioSource.cpp
 * @Author	Joakim Bertils
 * @date	2017-02-20
 * @brief	OpenAL audio source abstraction.
 */

#include "AudioSource.h"

AudioSource::AudioSource(const std::string& path, ALboolean looping)
	: buffer{ path }
{
	ALenum error;

	// Dummy read
	alGetError();

	alGenSources(1, &handle);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		throw AudioException("Could not generate buffer.");
	}

	// Attach the buffer to this source.
	alSourcei(handle, AL_BUFFER, buffer.getHandle());
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		throw AudioException("Could not attach buffer to source");
	}

	setLooping(looping);
}

AudioSource::~AudioSource()
{
	if(alIsSource(handle))
	{
		alDeleteSources(1, &handle);
	}
}

void AudioSource::play()
{
	alSourcePlay(handle);
}

void AudioSource::stop()
{
	alSourceStop(handle);
}

void AudioSource::pause()
{
	alSourcePause(handle);
}

void AudioSource::rewind()
{
	alSourceRewind(handle);
}

ALuint AudioSource::getHandle() const
{
	return handle;
}

ALfloat AudioSource::getPitch() const
{
	return pitch;
}

void AudioSource::setPitch(ALfloat pitch)
{
	this->pitch = pitch;

	ALenum error;
	alGetError();
	alSourcef(handle, AL_PITCH, pitch);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		throw AudioException("Could not set pitch value");
	}
}

ALfloat AudioSource::getGain() const
{
	return gain;
}

void AudioSource::setGain(ALfloat gain)
{
	this->gain = gain;

	ALenum error;
	alGetError();
	alSourcef(handle, AL_GAIN, gain);
	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		throw AudioException("Could not set gain value");
	}
}

glm::vec3 AudioSource::getPosition() const
{
	return position;
}

void AudioSource::setPosition(const glm::vec3& position)
{
	this->position = position;

	ALenum error;
	alGetError();
	alSource3f(handle, AL_POSITION, position.x, position.y, position.z);

	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		throw AudioException("Could not set position value");
	}
}

glm::vec3 AudioSource::getVelocity() const
{
	return velocity;
}

void AudioSource::setVelocity(const glm::vec3& velocity)
{
	this->velocity = velocity;

	ALenum error;
	alGetError();
	alSource3f(handle, AL_VELOCITY, velocity.x, velocity.y, velocity.z);

	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		throw AudioException("Could not set velocity value");
	}
}

glm::vec3 AudioSource::getDirection() const
{
	return direction;
}

void AudioSource::setDirection(const glm::vec3& direction)
{
	this->direction = direction;

	ALenum error;
	alGetError();
	alSource3f(handle, AL_DIRECTION, direction.x, direction.y, direction.z);

	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		throw AudioException("Could not set direction value");
	}
}

ALboolean AudioSource::getLooping() const
{
	return looping;
}

void AudioSource::setLooping(ALboolean looping)
{
	this->looping = looping;

	ALenum error;
	alGetError();
	alSourcei(handle, AL_LOOPING, looping);

	error = alGetError();
	if (error != AL_NO_ERROR)
	{
		throw AudioException("Could not set looping value");
	}
}