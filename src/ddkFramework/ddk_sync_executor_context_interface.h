#pragma once

#include "ddk_function.h"
#include "ddk_weak_pointer_wrapper.h"
#include "ddk_shared_pointer_wrapper.h"

namespace ddk
{

class executor_context_interface
{
public:
	virtual ~executor_context_interface() = default;

	virtual bool enqueue(const function<void()>&, char) = 0;
	virtual void clear() = 0;
};

typedef weak_pointer_wrapper<executor_context_interface> executor_context_weak_ptr;
typedef weak_pointer_wrapper<const executor_context_interface> executor_context_const_weak_ptr;

typedef shared_pointer_wrapper<executor_context_interface> executor_context_shared_ptr;
typedef shared_pointer_wrapper<const executor_context_interface> executor_context_const_shared_ptr;

typedef lent_reference_wrapper<executor_context_interface> executor_context_lent_ref;
typedef lent_reference_wrapper<const executor_context_interface> executor_context_const_lent_ref;
typedef lent_pointer_wrapper<executor_context_interface> executor_context_lent_ptr;
typedef lent_pointer_wrapper<const executor_context_interface> executor_context_const_lent_ptr;

}