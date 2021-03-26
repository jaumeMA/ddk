#pragma once

#include <string>
#include "ddk_type_id.h"

namespace ddk
{

size_t hash(const std::string& i_value);

#if !defined(WIN32)

constexpr size_t hash(const int& i_value)
{
    return static_cast<size_t>(i_value);
}
constexpr size_t hash(const unsigned long& i_value)
{
    return static_cast<size_t>(i_value);
}

#endif

constexpr size_t hash(const time_t& i_value)
{
    return static_cast<size_t>(i_value);
}
template<typename UnderlyingType, typename T>
constexpr size_t hash(const Id<UnderlyingType, T>& i_id)
{
	return hash(i_id.getValue());
}
template <class T>
constexpr void hash_combine(uint32_t& combined, const T& v)
{
  combined ^= (hash(v) + 0x9e3779b9) + (combined << 6) + (combined >> 2);
}
template <class T>
constexpr void hash_combine(uint64_t& combined, const T& v)
{
  combined ^= (hash(v) + 0x9e3779b9) + (combined << 6) + (combined >> 2);
}

}
