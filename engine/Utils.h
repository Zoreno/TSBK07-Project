/**
 * @file	Utils.h
 * @Author	Joakim Bertils
 * @date	2017-02-08
 * @brief	TODO: Insert documentation
 */

#pragma once

#include <string>

/**
 * @brief Constructs a string using content of file at path
 * @param path File path relative to Solution
 * @return String with file contents
 */
std::string getStringFromFile(const std::string& path);