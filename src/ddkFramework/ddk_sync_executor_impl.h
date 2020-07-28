#pragma once

#include "ddk_executor_interface.h"
#include "ddk_thread_sheaf.h"
#include "ddk_fiber_sheaf.h"
#include "ddk_fiber_yielder.h"

namespace ddk
{
namespace detail
{

template<typename Return>
class deferred_executor : public executor_interface<Return()>
{
public:
	deferred_executor() = default;

private:
	typedef typename executor_interface<Return()>::start_result start_result;

	start_result execute(const std::function<void(Return)>& i_sink, const std::function<Return()>& i_callable) override;
	ExecutorState get_state() const override;
};

template<typename Return>
class await_executor : public executor_interface<Return()>, public fiber_scheduler_interface, private fiber_yielder_interface, protected lend_from_this<await_executor<Return>,detail::fiber_scheduler_interface>
{
public:
	await_executor();
	await_executor(stack_allocator i_stackAlloc);
	~await_executor();

private:
	typedef typename executor_interface<Return()>::start_result start_result;

	start_result execute(const std::function<void(Return)>& i_sink, const std::function<Return()>& i_callable) override;
	ExecutorState get_state() const override;

	void yield(yielder_context* i_context) override;
	void suspend(yielder_context* = nullptr) override;

	bool activate(fiber_id, const std::function<void()>&) override;
	bool deactivate(fiber_id) override;
	void unregister(fiber_id) override;

	this_fiber_t m_caller;
	fiber_impl m_callee;
	detail::fiber_yielder m_yielder;
};

template<typename Return>
class fiber_executor : public executor_interface<Return()>
{
public:
	fiber_executor(fiber i_fiber);

private:
	typedef typename executor_interface<Return()>::start_result start_result;

	start_result execute(const std::function<void(Return)>& i_sink, const std::function<Return()>& i_callable) override;
	ExecutorState get_state() const override;

	mutable fiber m_fiber;
};

class fiber_sheaf_executor : public executor_interface<detail::void_t()>
{
public:
	fiber_sheaf_executor(fiber_sheaf i_fiberSheaf);
	~fiber_sheaf_executor() = default;

private:
	typedef typename executor_interface<detail::void_t()>::start_result start_result;

	start_result execute(const std::function<void(detail::void_t)>& i_sink, const std::function<detail::void_t()>& i_callable) override;
	ExecutorState get_state() const override;

	mutable fiber_sheaf m_fiberSheaf;
	size_t m_pendingFibers;
};

template<typename Return>
class thread_executor;

template<typename Return>
class thread_executor : public executor_interface<Return()>
{
public:
	thread_executor() = default;
	thread_executor(thread i_thread);

private:
	typedef typename executor_interface<Return()>::start_result start_result;

	start_result execute(const std::function<void(Return)>& i_sink, const std::function<Return()>& i_callable) override;
	ExecutorState get_state() const override;

	mutable thread m_thread;
};

class thread_sheaf_executor : public executor_interface<detail::void_t()>
{
public:
	thread_sheaf_executor(thread_sheaf i_threadSheaf);

private:
	typedef typename executor_interface<detail::void_t()>::start_result start_result;

	start_result execute(const std::function<void(detail::void_t)>& i_sink, const std::function<detail::void_t()>& i_callable) override;
	ExecutorState get_state() const override;

	mutable thread_sheaf m_threadSheaf;
	size_t m_pendingThreads;
};

}
}

#include "ddk_sync_executor_impl.inl"
