#pragma once

#include "ddk_void.h"
#include "ddk_crash.h"

namespace ddk
{
namespace detail
{

struct static_visitor_base{};

}

template<typename>
class static_visitor;

template<typename Return>
class static_visitor : detail::static_visitor_base
{
public:
	typedef Return return_type;

	template<typename T>
	Return visit(const T&)
	{
		static_assert(sizeof(T) == 0, "You shall provide visit method for this type");

		return ddk::crash_on_return<Return>::value();
	}
	template<typename T>
	Return visit(const T&) const
	{
		static_assert(sizeof(T) == 0, "You shall provide visit method for this type");

		return ddk::crash_on_return<Return>::value();
	}

};

template<>
class static_visitor<void> : detail::static_visitor_base
{
public:
	typedef detail::void_t return_type;

	template<typename T>
	return_type visit(T&&)
	{
		static_assert(sizeof(T) == 0, "You shall provide visit method for this type");

		return ddk::crash_on_return<return_type>::value();
	}
	template<typename T>
	return_type visit(T&&) const
	{
		static_assert(sizeof(T) == 0, "You shall provide visit method for this type");

		return ddk::crash_on_return<return_type>::value();
	}

};

}
