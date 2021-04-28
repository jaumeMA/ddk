#pragma once

#include "ddk_function.h"
#include "ddk_lent_reference_wrapper.h"

namespace ddk
{

class executor_context_interface
{
public:
	virtual ~executor_context_interface() = default;

	virtual void enqueue(const function<void()>&) = 0;
	virtual void clear() = 0;
};

typedef lent_reference_wrapper<executor_context_interface> executor_context_lent_ref;
typedef lent_reference_wrapper<const executor_context_interface> executor_context_const_lent_ref;
typedef lent_pointer_wrapper<executor_context_interface> executor_context_lent_ptr;
typedef lent_pointer_wrapper<const executor_context_interface> executor_context_const_lent_ptr;

}