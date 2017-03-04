/**
 * @file	AudioException.h
 * @Author	Joakim Bertils
 * @date	2017-02-18
 * @brief	Audio Exception class for exceptions in audio module
 */

#pragma once

#include <stdexcept>

/**
 * @brief Class for representing audio errors
 */
class AudioException : public std::logic_error
{
public:
	using std::logic_error::logic_error;
};