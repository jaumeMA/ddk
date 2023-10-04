#pragma once

#include "ddk_executor_interface.h"
#include "ddk_sync_executor_context.h"

namespace ddk
{

class promised_context : public ddk::lend_from_this<promised_context,detail::execution_context_base>
{
public:
	promised_context() = default;
	promised_context(promised_context&&) = default;

	void notify_recipients();
	void transfer_recipients(promised_context i_context);
	void clear_recipients();

private:
	void start(const function<void()>&);
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE(Callable))
	ddk::continuation_token enqueue(Callable&& i_callable, unsigned char i_depth);
	void transfer(detail::execution_context_base&& other);
	bool dismiss(unsigned char i_depth, continuation_token i_token);
	void clear();

	detail::async_executor_recipients m_recipients;
};

typedef atomic_distributed_reference_wrapper<promised_context> promised_context_dist_ref;
typedef atomic_distributed_reference_wrapper<const promised_context> promised_context_dist_const_ref;
typedef atomic_distributed_pointer_wrapper<promised_context> promised_context_dist_ptr;
typedef atomic_distributed_pointer_wrapper<const promised_context> promised_context_dist_const_ptr;

}

#include "ddk_promised_context.inl"