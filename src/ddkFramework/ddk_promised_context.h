#pragma once

#include "ddk_executor_interface.h"
#include "ddk_sync_executor_context.h"

namespace ddk
{

class promised_context : public executor_context_interface, public ddk::lend_from_this<promised_context,executor_context_interface>
{
public:
	promised_context() = default;
	promised_context(promised_context&&) = default;

	void notify_recipients();
	void clear_recipients();

private:
	void start(const function<void()>&) override;
	ddk::continuation_token enqueue(const function<void()>&, unsigned char i_depth) override;
	bool dismiss(unsigned char i_depth, continuation_token i_token) override;
	void clear() override;

	detail::async_executor_recipients m_recipients;
};

typedef atomic_distributed_reference_wrapper<promised_context> promised_context_dist_ref;
typedef atomic_distributed_reference_wrapper<const promised_context> promised_context_dist_const_ref;
typedef atomic_distributed_pointer_wrapper<promised_context> promised_context_dist_ptr;
typedef atomic_distributed_pointer_wrapper<const promised_context> promised_context_dist_const_ptr;

}