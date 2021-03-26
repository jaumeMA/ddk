#pragma once

#include "ddk_void.h"
#include "ddk_crash.h"

namespace ddk
{

template<typename>
class static_visitor;

template<typename Return>
class static_visitor
{
public:
	struct callable_tag;
	typedef Return return_type;

	template<typename T>
	Return operator()(const T&) const
	{
		static_assert(sizeof(T) == 0, "You shall provide visit method for this type");

		return ddk::crash_on_return<Return>::value();
	}

};

template<>
class static_visitor<void>
{
public:
	struct callable_tag;
	typedef detail::void_t return_type;

	template<typename T>
	return_type operator()(T&&) const
	{
		static_assert(sizeof(T) == 0, "You shall provide visit method for this type");

		return ddk::crash_on_return<return_type>::value();
	}

};

}
