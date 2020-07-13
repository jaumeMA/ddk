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
	typedef Return return_type;

	template<typename T>
	Return visit(T&&)
	{
		static_assert(sizeof(T) == 0, "You shall provide visit method for this type");

		return ewas::crash_on_return<Return>::value();
	}
	template<typename T>
	Return visit(T&&) const
	{
		static_assert(sizeof(T) == 0, "You shall provide visit method for this type");

		return ewas::crash_on_return<Return>::value();
	}

};

template<>
class static_visitor<void>
{
public:
	typedef detail::void_t return_type;

	template<typename T>
	return_type visit(T&&)
	{
		static_assert(sizeof(T) == 0, "You shall provide visit method for this type");

		return ewas::crash_on_return<return_type>::value();
	}
	template<typename T>
	return_type visit(T&&) const
	{
		static_assert(sizeof(T) == 0, "You shall provide visit method for this type");

		return ewas::crash_on_return<return_type>::value();
	}

};

}
