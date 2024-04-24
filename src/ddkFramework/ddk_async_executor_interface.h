#pragma once

#include "ddk_sync_executor_context.h"
#include "ddk_lent_reference_wrapper.h"
#include "ddk_shared_reference_wrapper.h"
#include "ddk_thread.h"
#include "ddk_fiber.h"
#include "ddk_embedded_type.h"
#include "ddk_result.h"

namespace ddk
{
namespace detail
{

struct private_async_state_base;

}

class async_interface_base
{
public:
	enum CancelErrorCode
	{
		CancelInternal,
		CancelNoAsync,
		CancelNoCallable,
		CancelAlreadyExecuted
	};
	typedef error<CancelErrorCode> cancel_error;
	typedef result<void,cancel_error> cancel_result;

	virtual ~async_interface_base() = default;
	virtual executor_context_lent_ptr get_execution_context() = 0;
	virtual executor_context_const_lent_ptr get_execution_context() const = 0;
	virtual allocator_const_lent_ptr get_async_allocator() const = 0;
	virtual cancel_result cancel() = 0;
	virtual void hold(shared_reference_wrapper<detail::private_async_state_base>) = 0;
};

using async_base_lent_ref = lent_reference_wrapper<async_interface_base>;
using async_base_const_lent_ref = lent_reference_wrapper<const async_interface_base>;
using async_base_lent_ptr = lent_pointer_wrapper<async_interface_base>;
using async_base_const_lent_ptr = lent_pointer_wrapper<const async_interface_base>;

using async_base_dist_ref = distributed_reference_wrapper<async_interface_base>;
using async_base_const_dist_ref = distributed_reference_wrapper<const async_interface_base>;
using async_base_dist_ptr = distributed_pointer_wrapper<async_interface_base>;
using async_base_const_dist_ptr = distributed_pointer_wrapper<const async_interface_base>;

template<typename,typename,typename,typename,typename>
class async_executor;

}
