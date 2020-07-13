#pragma once

#include "any_value.h"

namespace ddk
{

struct visitor_empty_value_t
{};
const visitor_empty_value_t visitor_empty_value = visitor_empty_value_t();

struct dynamic_visitor
{
	virtual ~dynamic_visitor(){};
	template<typename T>
	any_value visit(T&&) const
	{
		static_assert(sizeof(T)==0, "You have to provide a visit method for this type");

		return visitor_empty_value;
	}
	template<typename T>
	any_value visit(T&&)
	{
		static_assert(sizeof(T)==0, "You have to provide a visit method for this type");

		return visitor_empty_value;
	}
};

}