#pragma once

#include "ewas_fiber_defs.h"

namespace ewas
{
namespace detail
{

fiber_id& __get_current_fiber_id();
fiber_id get_next_available_fiber_id();

}

void set_current_fiber_id(const fiber_id&);

fiber_id get_current_fiber_id();

fiber_id get_thread_fiber_id();

void suspend();

void yield();

template<typename T>
void yield(T&& i_value);

}

#include "ewas_fiber_utils.inl"