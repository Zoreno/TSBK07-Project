#pragma once

#include <AL/al.h>
#include <glm/glm.hpp>

#include "AudioException.h"

class AudioListener
{
public:
	AudioListener();
	~AudioListener();

	AudioListener(const AudioListener&) = delete;
	AudioListener(AudioListener&&) = delete;

	const AudioListener& operator=(const AudioListener&) = delete;
	AudioListener& operator=(AudioListener&&) = delete;

	ALfloat getGain() const;
	void setGain(ALfloat gain);
	glm::vec3 getPosition() const;
	void setPosition(const glm::vec3& position);
	glm::vec3 getVelocity() const;
	void setVelocity(const glm::vec3& velocity);
	glm::vec3 getOrientationAt() const;
	void setOrientationAt(const glm::vec3& orientationAt);
	glm::vec3 getOrientationUp() const;
	void setOrientationUp(const glm::vec3& orientationUp);

private:

	void updateOrientation(ALfloat* vector);

	ALfloat gain{ 1.f };

	glm::vec3 position{};
	glm::vec3 velocity{};
	glm::vec3 orientationAt{ 0.f, 0.f, -1.f };
	glm::vec3 orientationUp{ 0.f, 1.f, 0.f };
};