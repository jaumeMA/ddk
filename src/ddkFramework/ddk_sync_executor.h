#pragma once

#include "ddk_async_executor_interface.h"
#include "ddk_promise.h"
#include "ddk_sync_executor_impl.h"

namespace ddk
{

template<typename Return>
class async_executor : public async_execute_interface<Return>
{
	template<typename RReturn>
	friend shared_reference_wrapper<async_executor<RReturn>> make_async_executor(const ddk::function<RReturn()>& i_function);
	friend class future<Return>;

	async_executor() = default;
	async_executor(const ddk::function<Return()>& i_function);
	using typename async_execute_interface<Return>::StartErrorCode;
	typedef typename executor_interface<Return()>::start_result nested_start_result;

public:
	typedef shared_reference_wrapper<async_executor<Return>> async_shared_ref;
	typedef shared_reference_wrapper<const async_executor<Return>> async_const_shared_ref;
	using typename async_execute_interface<Return>::start_result;
	using typename async_cancellable_interface<Return>::cancel_result;
	typedef typename promise<Return>::reference reference;
	typedef typename promise<Return>::const_reference const_reference;
	typedef typename promise<Return>::value_type value_type;

	async_executor(const async_executor&) = delete;
	async_executor(async_executor&& other);
	~async_executor();
	async_executor& operator=(const async_executor&) = delete;

	async_shared_ref attach(thread i_thread);
	async_shared_ref attach(const detail::this_thread_t&);
	async_shared_ref attach(fiber i_fiber);
	async_shared_ref attach(const detail::this_fiber_t&);
	shared_reference_wrapper<async_executor<detail::void_t>> attach(thread_sheaf i_threadSheaf);
	shared_reference_wrapper<async_executor<detail::void_t>> attach(fiber_sheaf i_fiberSheaf);
	async_shared_ref attach(executor_unique_ptr<Return> i_executor);
	async_shared_ref store(promise<Return>& i_promise);
	async_shared_ref on_cancel(const ddk::function<bool()>& i_cancelFunc);
	void bind();

private:
	cancel_result cancel() override;
	reference get_value() override;
	const_reference get_value() const override;
	value_type extract_value() override;
	void wait() const override;
	void wait_for(unsigned int i_period) const override;
	bool ready() const override;
	start_result execute() override;

	void set_value(Return i_value);

	ddk::function<Return()> m_function;
	ddk::function<bool()> m_cancelFunc;
	cancellable_executor_unique_ptr<Return> m_executor;
	mutable detail::private_async_state_shared_ptr<Return> m_sharedState;
	shared_reference_counter m_refCounter;
};

template<>
class async_executor<void> : public async_executor<detail::void_t>
{
public:
	typedef shared_reference_wrapper<async_executor<detail::void_t>> async_shared_ref;
	typedef shared_reference_wrapper<const async_executor<detail::void_t>> async_const_shared_ref;
};

}

#include "ddk_sync_executor.inl"
