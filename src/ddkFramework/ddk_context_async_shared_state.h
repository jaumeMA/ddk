#pragma once

#include "ddk_sync_executor_context.h"

namespace ddk
{
namespace detail
{

template<typename T>
struct context_private_async_state : public execution_context_base, private_async_state<T>
{
public:
	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	context_private_async_state(Args&& ... i_args);
};

template<typename T>
using context_private_async_state_shared_ref = shared_reference_wrapper<context_private_async_state<T>>;
template<typename T>
using context_private_async_state_const_shared_ref = shared_reference_wrapper<const context_private_async_state<T>>;
template<typename T>
using context_private_async_state_shared_ptr = shared_pointer_wrapper<context_private_async_state<T>>;
template<typename T>
using context_private_async_state_const_shared_ptr = shared_pointer_wrapper<const context_private_async_state<T>>;

template<typename T>
using context_private_async_state_weak_ptr = weak_pointer_wrapper<context_private_async_state<T>>;
template<typename T>
using context_private_async_state_const_weak_ptr = weak_pointer_wrapper<const context_private_async_state<T>>;

template<typename T>
using context_private_async_state_lent_ref = lent_reference_wrapper<context_private_async_state<T>>;
template<typename T>
using context_private_async_state_const_lent_ref = lent_reference_wrapper<const context_private_async_state<T>>;
template<typename T>
using context_private_async_state_lent_ptr = lent_pointer_wrapper<context_private_async_state<T>>;
template<typename T>
using context_private_async_state_const_lent_ptr = lent_pointer_wrapper<const context_private_async_state<T>>;

}
}

#include "ddk_context_async_shared_state.inl"