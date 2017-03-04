/**
 * @file	ConfigFile.cpp
 * @Author	Joakim Bertils
 * @date	2017-02-08
 * @brief	Implementation of the simple configuration parser.
 */

#include "ConfigFile.h"

#include <fstream>

ConfigFile::ConfigFile(const std::string& path)
	:path{ path }
{
	parse();
}

bool ConfigFile::parse()
{
	if(path.empty())
	{
		return false;
	}

	std::ifstream fileStream{ path };

	if(!fileStream.is_open())
	{
		return false;
	}

	std::string line;

	data.clear();

	while(std::getline(fileStream, line))
	{
		std::string varName;
		std::string varValue;

		if (line.substr(0, 2) == std::string("//"))
			continue;

		if (line.empty())
			continue;

		varName = line.substr(0, line.find(" "));
		varValue = line.substr(line.find(" ") + 1);

		data.emplace(varName, varValue);
	}

	fileStream.close();

	return true;
}

bool ConfigFile::parse(const std::string& path)
{
	this->path = path;
	return parse();
}

bool ConfigFile::hasKey(const std::string& key) const
{
	return (data.find(key) != data.end());
}

std::string ConfigFile::get(const std::string& key) const
{
	auto it = data.find(key);
	if(it != data.end())
	{
		return it->second;	
	}

	throw ConfigFileKeyException{ std::string("Could not find key:") + key };
}