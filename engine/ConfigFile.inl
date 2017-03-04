/**
 * @file	ConfigFile.inl
 * @Author	Joakim Bertils
 * @date	2017-02-12
 * @brief	Inline definitions of members belonging to ConfigFile class
 */

template <typename T>
T ConfigFile::get(const std::string& key) const
{
	auto it = data.find(key);
	if (it != data.end())
	{
		std::stringstream ss{ it->second };

		T value;

		ss >> value;

		return value;
	}

	throw ConfigFileKeyException{ std::string("Could not find key:") + key };
}
