#pragma once

#include "ddk_fiber_defs.h"

namespace ddk
{
namespace detail
{

struct execution_context;

execution_context*& __get_current_execution_context();

}

fiber_id get_current_fiber_id();

void set_current_execution_context(detail::execution_context& i_ctxt);

inline detail::execution_context& get_current_execution_context()
{
	return *detail::__get_current_execution_context();
}

void suspend();

void yield();

template<typename T>
void yield(T&& i_value);

}

#include "ddk_fiber_utils.inl"
