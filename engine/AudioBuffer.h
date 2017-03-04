/**
 * @file	AudioBuffer.h
 * @Author	Joakim Bertils
 * @date	2017-02-18
 * @brief	OpenAL static audio buffer abstraction.
 */

#pragma once

#include <string>

#include <AL/al.h>

#include "AudioException.h"

/**
 * @brief Class Representing an openAL audio buffer
 */
class AudioBuffer
{
public:
	/**
	 * @brief Constructor.
	 * 
	 * Creates an audio buffer and fills it with wave data from @path
	 * 
	 * @param path The path to the wave file.
	 */
	explicit AudioBuffer(const std::string& path);
	
	/**
	 * @brief Destructor
	 */
	~AudioBuffer();

	/**
	 * @brief Copy Constructor
	 * 
	 * @note Deleted.
	 */
	AudioBuffer(const AudioBuffer&) = delete;
	
	/**
	* @brief Move Constructor
	*
	* @note Deleted.
	*/
	AudioBuffer(AudioBuffer&&) = delete;

	/**
	* @brief Copy Assignment Operator
	*
	* @note Deleted.
	*/
	const AudioBuffer& operator=(const AudioBuffer&) = delete;
	
	/**
	* @brief Move Assignment Operator
	*
	* @note Deleted.
	*/
	AudioBuffer& operator=(const AudioBuffer&&) = delete;

	/**
	 * @brief Gets the OpenAL handle for the buffer.
	 * @return Handle
	 */
	ALuint getHandle() const;

	/**
	 * @brief Gets the path to where the data was loaded from.
	 * @return Path to wave file.
	 */
	std::string getPath() const;
private:

	/**
	 * @brief Path to where the data was loaded from.
	 */
	std::string path{};

	/**
	 * @brief OpenAL Handle for the buffer.
	 */
	ALuint handle;
};