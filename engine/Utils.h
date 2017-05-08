/**
 * @file	Utils.h
 * @Author	Joakim Bertils
 * @date	2017-02-08
 * @brief	TODO: Insert documentation
 */

#pragma once

#include <string>
#include <glm/glm.hpp>
#include "EngineDLL.h"

/**
 * @brief Constructs a string using content of file at path
 * @param path File path relative to Solution
 * @return String with file contents
 */
ENGINE_API std::string getStringFromFile(const std::string& path);

ENGINE_API std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);

ENGINE_API std::istream& operator>>(std::istream& is, glm::vec3& vec);