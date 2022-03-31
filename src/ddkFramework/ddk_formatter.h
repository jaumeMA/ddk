#pragma once

#include <string>
#include <set>
#include <vector>
#include <list>
#include <map>
#include <utility>
#include <sstream>
#include "ddk_macros.h"

namespace ddk
{

template<typename T>
struct formatter
{
	inline static T format(const std::string& i_str);
};

template<>
struct formatter<bool>
{
	static bool format(const std::string& i_str);
};
template<>
struct formatter<char>
{
	static char format(const std::string& i_str);
};
template<>
struct formatter<unsigned char>
{
	static unsigned char format(const std::string& i_str);
};
template<>
struct formatter<short>
{
	static short format(const std::string& i_str);
};
template<>
struct formatter<unsigned short>
{
	static unsigned short format(const std::string& i_str);
};
template<>
struct formatter<int>
{
	static int format(const std::string& i_str);
};
template<>
struct formatter<unsigned int>
{
	static unsigned int format(const std::string& i_str);
};
template<>
struct formatter<long>
{
	static long format(const std::string& i_str);
};
template<>
struct formatter<unsigned long>
{
	static unsigned long format(const std::string& i_str);
};
template<>
struct formatter<long long>
{
	static long long format(const std::string& i_str);
};
template<>
struct formatter<unsigned long long>
{
	static unsigned long long format(const std::string& i_str);
};
template<>
struct formatter<float>
{
	static float format(const std::string& i_str);
};
template<>
struct formatter<double>
{
	static double format(const std::string& i_str);
};
template<>
struct formatter<std::string>
{
	static std::string format(const std::string& i_str);
	template<typename T>
	inline static std::string format(const T& i_val);
	template<typename T>
	inline static std::string format(const std::set<T>& i_val);
	template<typename T>
	inline static std::string format(const std::vector<T>& i_val);
	template<typename T>
	inline static std::string format(const std::list<T>& i_val);
	template<typename First,typename Second>
	inline static std::string format(const std::pair<First,Second>& i_val);
	template<typename Key, typename Value>
	inline static std::string format(const std::map<Key,Value>& i_val);
};

template<typename First,typename Second>
struct formatter<std::pair<First,Second>>
{
	inline static std::pair<First,Second> format(std::string i_str);
};

template<typename T>
struct formatter<std::vector<T>>
{
	inline static std::vector<T> format(std::string i_str);
};

template<typename T>
struct formatter<std::list<T>>
{
	inline static std::list<T> format(std::string i_str);
};

template<typename T>
struct formatter<std::set<T>>
{
	inline static std::set<T> format(std::string i_str);
};

template<typename Key, typename Value>
struct formatter<std::map<Key,Value>>
{
	inline static std::map<Key,Value> format(std::string i_str);
};

template<typename T>
inline T format_to(const std::string& i_value);

template<typename T>
inline std::string to_string(T&& i_value);

template<typename T, typename ... TT>
inline std::string to_string(T&& i_value, TT&& ... i_values);

}

#include "ddk_formatter.inl"