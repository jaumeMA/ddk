#pragma once

#include <vector>
#include <string>
#include <unordered_map>

namespace ewas
{
template<class value_t>
class property_map
{
protected:
	typedef std::unordered_map<std::string, value_t> map_t;

public:
	size_t size() const { return map.size(); }

	// Non-recursive version.
	bool get(const std::string &key, value_t &value) const
	{
		bool result = false;

		const typename map_t::const_iterator it = map.find(key);
		if(it != map.end())
		{
			value = it->second;
			result = true;
		}

		return result;
	}

	// Recursive version.
	value_t get(const std::string &key) const
	{
		typename map_t::const_iterator it = map.find(key);
		if(it != map.end())
			return it->second;

		std::string subkey(key);

		for(size_t i = subkey.rfind("."); i != std::string::npos; i = subkey.rfind("."))
		{
			subkey.resize(i);

			it = map.find(subkey);
			if(it != map.end())
				return it->second;
		}

		return value_t();
	}

	void set(const std::string &key, const value_t &value)
	{
		map[key] = value;
	}

	bool contains(const std::string &key) const
	{
		value_t value;

		return get(key, value);
	}

	bool erase(const std::string &key)
	{
		return map.erase(key) > 0;
	}

	void clear() { map.clear(); }

	bool operator ==(const property_map &properties) const { return map == properties.map; }
	bool operator !=(const property_map &properties) const { return !(*this == properties); }

private:
	map_t map;
};
}
