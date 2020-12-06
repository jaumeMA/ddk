#pragma once

#include "ddk_async_executor_interface.h"
#include "ddk_promise.h"
#include "ddk_sync_executor_impl.h"
#include "ddk_attachable.h"

namespace ddk
{

template<typename Return>
class async_executor : public async_execute_interface<Return>, public async_cancellable_interface
{
	template<typename RReturn>
	friend shared_reference_wrapper<async_executor<RReturn>> make_async_executor(const ddk::function<RReturn()>& i_function);
	friend class future<Return>;
	using typename async_execute_interface<Return>::StartErrorCode;
	typedef typename executor_interface<Return()>::start_result nested_start_result;
	typedef typename executor_interface<Return()>::sink_reference sink_reference;

public:
	typedef shared_reference_wrapper<async_executor<Return>> async_shared_ref;
	typedef shared_reference_wrapper<const async_executor<Return>> async_const_shared_ref;
	using typename async_execute_interface<Return>::start_result;
	using typename async_cancellable_interface::cancel_result;
	typedef typename detail::private_async_state<Return>::reference reference;
	typedef typename detail::private_async_state<Return>::const_reference const_reference;
	typedef typename detail::private_async_state<Return>::rref_type rref_type;

	async_executor(const async_executor&) = delete;
	async_executor(async_executor&& other);
	~async_executor();
	async_executor& operator=(const async_executor&) = delete;

	async_shared_ref attach(thread i_thread);
	async_shared_ref attach(fiber i_fiber);
	shared_reference_wrapper<async_executor<detail::void_t>> attach(thread_sheaf i_threadSheaf);
	shared_reference_wrapper<async_executor<detail::void_t>> attach(fiber_sheaf i_fiberSheaf);
	async_shared_ref attach(attachable<Return> i_attachable);
	async_shared_ref store(promise<Return>& i_promise);
	async_shared_ref on_cancel(const ddk::function<bool()>& i_cancelFunc);
	future<Return> as_future();

protected:
	async_executor() = default;
    async_executor(const function<Return()>& i_function);

private:
	bool notify() override;
	start_result execute() override;
	cancel_result cancel() override;

	void set_value(sink_reference i_value);

	ddk::function<Return()> m_function;
	ddk::function<bool()> m_cancelFunc;
	cancellable_executor_unique_ptr<Return> m_executor;
	mutable promise<Return> m_promise;
	shared_reference_counter m_refCounter;
};

template<>
class async_executor<void> : public async_executor<detail::void_t>
{
	template<typename Return>
	friend shared_reference_wrapper<async_executor<Return>> make_async_executor(const ddk::function<Return()>& i_function);
	friend class future<void>;

public:
	typedef shared_reference_wrapper<async_executor<detail::void_t>> async_shared_ref;
	typedef shared_reference_wrapper<const async_executor<detail::void_t>> async_const_shared_ref;

private:
    async_executor(const function<void()>& i_function)
    : async_executor<detail::void_t>([i_function]() -> detail::void_t { eval(i_function); return _void; })
    {}
};

}

#include "ddk_sync_executor.inl"
