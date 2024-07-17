//////////////////////////////////////////////////////////////////////////////
//
// Author: Jaume Moragues
// Distributed under the GNU Lesser General Public License, Version 3.0. (See a copy
// at https://www.gnu.org/licenses/lgpl-3.0.ca.html)
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ddk_sync_executor_context.h"

namespace ddk
{
namespace detail
{

template<typename T>
struct context_private_async_state : public execution_context_base, private_async_state<T>, public async_interface_base, public ddk::distribute_from_this<context_private_async_state<T>>
{
public:
	using async_interface_base::cancel_result;

	TEMPLATE(typename ... Args)
	REQUIRES(IS_CONSTRUCTIBLE(T,Args...))
	context_private_async_state(Args&& ... i_args);
	~context_private_async_state();

private:
	executor_context_lent_ptr get_execution_context() override;
	executor_context_const_lent_ptr get_execution_context() const override;
	allocator_const_lent_ptr get_async_allocator() const override;
	cancel_result cancel() override;
	void hold(private_async_state_base_shared_ref i_sharedState) override;
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