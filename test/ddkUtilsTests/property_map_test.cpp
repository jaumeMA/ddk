#include <gtest/gtest.h>

#include "property_map.h"

using namespace ewas;
using namespace testing;

class PropertyMapTest : public Test
{
public:
	PropertyMapTest() :
		keys({
			"aircraft",
			"airport",
			"aircraft.airport",
			"airport.aircraft",
			"airport.level",
			"airport.aircraft.level"
		})
	{}

	const std::vector<std::string> keys;
};

TEST_F(PropertyMapTest, GetEmpty)
{
	property_map<int> map;

	for(size_t i = 0; i < keys.size(); ++i)
	{
		EXPECT_EQ(map.get(keys[i]), 0);
	}
}

TEST_F(PropertyMapTest, GetSet)
{
	property_map<int> map;

	int value = 0;

	for(size_t i = 0; i < keys.size(); ++i)
	{
		map.set(keys[i], ++value);

		EXPECT_EQ(map.get(keys[i]), value);
	}
}

TEST_F(PropertyMapTest, GetNonRecursive)
{
	const std::string key = "airport.aircraft.level";

	const int valid = 1, invalid = -1;

	property_map<int> map;

	map.set(key, valid);

	for(size_t i = 0; i < keys.size(); ++i)
	{
		int value = invalid;

		if(keys[i] == key)
		{
			EXPECT_TRUE(map.get(keys[i], value));
			EXPECT_EQ(value, valid);
		}
		else
		{
			EXPECT_FALSE(map.get(keys[i], value));
			EXPECT_EQ(value, invalid);
		}
	}
}

TEST_F(PropertyMapTest, GetRecursive)
{
	const std::string key = "airport.aircraft.level";

	property_map<int> map;

	int value = 0;

	for(size_t i = 0; i < keys.size(); ++i)
	{
		map.set(keys[i], ++value);

		if(keys[i] == key.substr(0, keys[i].size()))
			EXPECT_EQ(map.get(key), value);
		else
			EXPECT_NE(map.get(key), value);
	}
}

TEST_F(PropertyMapTest, Contains)
{
	const std::string key = "airport.aircraft.level";

	property_map<int> map;

	map.set(key, 0);

	for(size_t i = 0; i < keys.size(); ++i)
	{
		if(keys[i] == key)
			EXPECT_TRUE(map.contains(keys[i]));
		else
			EXPECT_FALSE(map.contains(keys[i]));
	}
}

TEST_F(PropertyMapTest, Erase)
{
	const std::string key = "airport.aircraft.level";

	property_map<int> map;

	map.set(key, 0);

	for(size_t i = 0; i < keys.size(); ++i)
	{
		if(keys[i] == key)
			EXPECT_TRUE(map.erase(keys[i]));
		else
			EXPECT_FALSE(map.erase(keys[i]));
	}
}
