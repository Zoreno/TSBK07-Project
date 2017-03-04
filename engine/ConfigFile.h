/**
 * @file	ConfigFile.h
 * @Author	Joakim Bertils
 * @date	2017-02-08
 * @brief	Interface of a very simple Config file parser
 */

#pragma once

#include <string>
#include <map>
#include <stdexcept>
#include <sstream>

/**
 * @brief Configuration file key not found exception
 */
class ConfigFileKeyException : std::logic_error
{
public:
	using std::logic_error::logic_error;
};


/**
 * @brief Very simple configuration file parser
 * 
 * Format for the config file is:
 * 
 * key value
 * 
 * The key may be a string without spaces and the value is the rest of the line.
 * Only one entry per line.
 * Blank lines and lines beginning with // are skipped.
 */
class ConfigFile
{
public:
	/**
	 * @brief Default constructor
	 */
	ConfigFile() = default;

	/**
	 * @brief Constructs and parses the file at path
	 * @param path Path to the file
	 */
	explicit ConfigFile(const std::string& path);

	/**
	 * @brief Parses the currently stored path
	 * @return True if successful
	 */
	bool parse();


	/**
	 * @brief Sets a new path and parses the new file.
	 * @param path New path to be parsed.
	 * @return True if successful
	 */
	bool parse(const std::string& path);

	/**
	 * @brief Checks if the configFile contains a specific key.
	 * @param key The key to check for.
	 * @return True if found.
	 */
	bool hasKey(const std::string& key) const;

	/**
	 * @brief Gets the value for the specified key.
	 * 
	 * Throws an exception if key not found.
	 * 
	 * @param key The key to check for.
	 * @return Value for key.
	 */
	std::string get(const std::string& key) const;


	/**
	 * @brief Gets the value for the specified key in specified type.
	 * 
	 * Uses a std::stringstream to parse string content to type @T. 
	 * 
	 * @tparam T Type for the value to be interpreted as.
	 * @param key The key to check for.
	 * @return Value for the key.
	 */
	template <typename T>
	T get(const std::string& key) const;
private:

	/**
	 * @brief The currently stored file path.
	 */
	std::string path{};

	/**
	 * @brief Map containing the data in the file.
	 */
	std::map<std::string, std::string> data{};
};

#include "ConfigFile.inl"