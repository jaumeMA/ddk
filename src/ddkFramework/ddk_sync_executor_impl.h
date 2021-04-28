#pragma once

#include "ddk_executor_interface.h"
#include "ddk_sync_executor_context.h"
#include "ddk_lendable.h"
#include "ddk_fiber_yielder.h"

namespace ddk
{
namespace detail
{

template<typename Return>
class deferred_executor : public cancellable_executor_interface<Return()>
{
public:
	deferred_executor();

private:
	typedef typename executor_interface<Return()>::sink_reference sink_reference;
    using typename cancellable_executor_interface<Return()>::start_result;
    using typename cancellable_executor_interface<Return()>::StartErrorCode;
    using typename cancellable_executor_interface<Return()>::cancel_result;
    using typename cancellable_executor_interface<Return()>::CancelErrorCode;

	start_result execute(const ddk::function<void(sink_reference)>& i_sink, const ddk::function<Return()>& i_callable) override;
	cancel_result cancel(const ddk::function<bool()>& i_cancelFunc) override;
	executor_context_lent_ref get_execution_context() override;
	executor_context_const_lent_ref get_execution_context() const override;
	ExecutorState get_state() const override;

	atomic<ExecutorState::underlying_type> m_state;
	lendable<deferred_execution_context> m_execContext;
};

template<typename Return>
class fiber_executor : public cancellable_executor_interface<Return()>
{
public:
	fiber_executor(fiber i_fiber);

private:
	typedef typename executor_interface<Return()>::sink_reference sink_reference;
    using typename cancellable_executor_interface<Return()>::start_result;
    using typename cancellable_executor_interface<Return()>::StartErrorCode;
    using typename cancellable_executor_interface<Return()>::cancel_result;
    using typename cancellable_executor_interface<Return()>::CancelErrorCode;

	start_result execute(const ddk::function<void(sink_reference)>& i_sink, const ddk::function<Return()>& i_callable) override;
	cancel_result cancel(const ddk::function<bool()>& i_cancelFunc) override;
	executor_context_lent_ref get_execution_context() override;
	executor_context_const_lent_ref get_execution_context() const override;
	ExecutorState get_state() const override;

	lendable<fiber_execution_context> m_execContext;
	atomic<ExecutorState::underlying_type> m_state;
};

class fiber_sheaf_executor : public cancellable_executor_interface<detail::void_t()>
{
public:
	fiber_sheaf_executor(fiber_sheaf i_fiberSheaf);

private:
	typedef typename executor_interface<detail::void_t()>::sink_reference sink_reference;
    using typename cancellable_executor_interface<void_t()>::start_result;
    using typename cancellable_executor_interface<void_t()>::StartErrorCode;
    using typename cancellable_executor_interface<void_t()>::cancel_result;
    using typename cancellable_executor_interface<void_t()>::CancelErrorCode;

	start_result execute(const ddk::function<void(const detail::void_t&)>& i_sink, const ddk::function<detail::void_t()>& i_callable) override;
	cancel_result cancel(const ddk::function<bool()>& i_cancelFunc) override;
	executor_context_lent_ref get_execution_context() override;
	executor_context_const_lent_ref get_execution_context() const override;
	ExecutorState get_state() const override;

	lendable<fiber_sheaf_execution_context> m_execContext;
	atomic32<ExecutorState::underlying_type> m_state;
};

template<typename Return>
class thread_executor;

template<typename Return>
class thread_executor : public cancellable_executor_interface<Return()>
{
public:
	thread_executor(thread i_thread);

private:
	typedef typename executor_interface<Return()>::sink_reference sink_reference;
    using typename cancellable_executor_interface<Return()>::start_result;
    using typename cancellable_executor_interface<Return()>::StartErrorCode;
    using typename cancellable_executor_interface<Return()>::cancel_result;
    using typename cancellable_executor_interface<Return()>::CancelErrorCode;

	start_result execute(const ddk::function<void(sink_reference)>& i_sink, const ddk::function<Return()>& i_callable) override;
	cancel_result cancel(const ddk::function<bool()>& i_cancelFunc) override;
	executor_context_lent_ref get_execution_context() override;
	executor_context_const_lent_ref get_execution_context() const override;
	ExecutorState get_state() const override;

	lendable<thread_execution_context> m_execContext;
	atomic<ExecutorState::underlying_type> m_state;
};

class thread_sheaf_executor : public cancellable_executor_interface<detail::void_t()>
{
public:
	thread_sheaf_executor(thread_sheaf i_threadSheaf);

private:
	typedef typename executor_interface<detail::void_t()>::sink_reference sink_reference;
    using typename cancellable_executor_interface<void_t()>::start_result;
    using typename cancellable_executor_interface<void_t()>::StartErrorCode;
    using typename cancellable_executor_interface<void_t()>::cancel_result;
    using typename cancellable_executor_interface<void_t()>::CancelErrorCode;

	start_result execute(const ddk::function<void(const detail::void_t&)>& i_sink, const ddk::function<detail::void_t()>& i_callable) override;
	cancel_result cancel(const ddk::function<bool()>& i_cancelFunc) override;
	executor_context_lent_ref get_execution_context() override;
	executor_context_const_lent_ref get_execution_context() const override;
	ExecutorState get_state() const override;

	lendable<thread_sheaf_execution_context> m_execContext;
	atomic<ExecutorState::underlying_type> m_state;
};

template<typename Return>
class execution_context_executor : public cancellable_executor_interface<Return()>
{
public:
	execution_context_executor(executor_context_lent_ref i_execContext);

private:
	typedef typename executor_interface<Return()>::sink_reference sink_reference;
	using typename cancellable_executor_interface<Return()>::start_result;
	using typename cancellable_executor_interface<Return()>::StartErrorCode;
	using typename cancellable_executor_interface<Return()>::cancel_result;
	using typename cancellable_executor_interface<Return()>::CancelErrorCode;

	start_result execute(const ddk::function<void(sink_reference)>& i_sink,const ddk::function<Return()>& i_callable) override;
	cancel_result cancel(const ddk::function<bool()>& i_cancelFunc) override;
	executor_context_lent_ref get_execution_context() override;
	executor_context_const_lent_ref get_execution_context() const override;
	ExecutorState get_state() const override;

	executor_context_lent_ref m_execContext;
	atomic<ExecutorState::underlying_type> m_state;
};

}
}

#include "ddk_sync_executor_impl.inl"
