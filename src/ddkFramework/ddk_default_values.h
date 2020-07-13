#pragma once

#include <time.h>
#include "WASTypes.h"
#include <string>
#include "type_id.h"

namespace ddk
{

template<typename T>
struct default_value
{
	static inline T initial_value()
	{
		return T();
	}
};

template<>
struct default_value<bool>
{
	static inline bool initial_value()
	{
		return false;
	}
	static inline bool invalid_value()
	{
		return false;
	}
};
template<>
struct default_value<time_t>
{
	static inline time_t initial_value()
	{
		return 0;
	}
	static inline time_t invalid_value()
	{
		return -1;
	}
};
template<>
struct default_value<was_uint32>
{
	static inline was_uint32 initial_value()
	{
		return 0;
	}
	static inline was_uint32 invalid_value()
	{
		return -1;
	}
};
template<>
struct default_value<size_t>
{
	static inline size_t initial_value()
	{
		return 0;
	}
	static inline size_t invalid_value()
	{
		return -1;
	}
};
template<>
struct default_value<was_uint16>
{
	static inline was_uint16 initial_value()
	{
		return 0;
	}
	static inline was_uint16 invalid_value()
	{
		return -1;
	}
};
template<>
struct default_value<int>
{
	static inline int initial_value()
	{
		return 0;
	}
	static inline int invalid_value()
	{
		return -1;
	}
};
template<>
struct default_value<char>
{
	static inline char initial_value()
	{
		return 0;
	}
	static inline char invalid_value()
	{
		return '\0';
	}
};
template<>
struct default_value<float>
{
	static inline float initial_value()
	{
		return 0.f;
	}
	static inline float invalid_value()
	{
		return -1.f;
	}
};
template<>
struct default_value<double>
{
	static inline double initial_value()
	{
		return 0;
	}
	static inline double invalid_value()
	{
		return -1;
	}
};
template<>
struct default_value<std::string>
{
	static inline std::string initial_value()
	{
		return "";
	}
	static inline std::string invalid_value()
	{
		return "";
	}
};
template<typename UnderlyingType, typename T>
struct default_value<Id<UnderlyingType,T>>
{
	static inline Id<UnderlyingType,T> initial_value()
	{
		return Id<UnderlyingType,T>(default_value<UnderlyingType>::initial_value());
	}
	static inline Id<UnderlyingType,T> invalid_value()
	{
		return Id<UnderlyingType,T>(default_value<UnderlyingType>::invalid_value());
	}
};

}