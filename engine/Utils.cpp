/**
 * @file	Utils.cpp
 * @Author	Joakim Bertils
 * @date	2017-02-08
 * @brief	TODO: Insert documentation
 */

#include "Utils.h"

#include <fstream>
#include <streambuf>

std::string getStringFromFile(const std::string& path)
{
	// Stream iterator magic
	return std::string{ std::istreambuf_iterator<char>(std::ifstream(path)), std::istreambuf_iterator<char>() };
}