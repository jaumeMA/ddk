#pragma once

#include "ddk_sync_executor_context_interface.h"
#include "ddk_mutex.h"
#include "ddk_thread.h"
#include "ddk_fiber.h"
#include "ddk_thread_sheaf.h"
#include "ddk_fiber_sheaf.h"
#include "ddk_lend_from_this.h"
#include <queue>
#include <list>

namespace ddk
{
namespace detail
{

struct async_executor_recipients : public lend_from_this<async_executor_recipients>
{

struct task
{
	friend inline void eval(const task& i_task)
	{
		eval(i_task.m_callable);
	}

public:
	task(size_t i_token, function<void()> i_callable);

	bool operator==(const continuation_token& i_token) const;

private:
	size_t m_token;
	function<void()> m_callable;
};

	typedef std::map<unsigned char,std::list<task>> callable_container;

public:
	async_executor_recipients();
	async_executor_recipients(async_executor_recipients&& other);

	async_executor_recipients& operator=(async_executor_recipients&& other);
	void notify(bool i_useAndKeep);
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE(Callable))
	continuation_token accept(Callable&& i_callable, unsigned char i_depth);
	bool dismiss(unsigned char i_depth, continuation_token i_token);
	bool move(async_executor_recipients&& i_recipients);
	void clear();

protected:
	bool is_admissible();
	void resolve(callable_container& i_pendingCallables);

	bool m_destroyAfterUse;
	mutex m_mutex;
	callable_container m_pendingCallables;
	bool m_admissible = true;
};
typedef lent_reference_wrapper<async_executor_recipients> execution_recipients_ref;
typedef lent_reference_wrapper<const async_executor_recipients> execution_recipients_const_ref;
typedef lent_pointer_wrapper<async_executor_recipients> execution_recipients_ptr;
typedef lent_pointer_wrapper<const async_executor_recipients> execution_recipients_const_ptr;

class execution_context_base
{
public:
	execution_context_base();
	execution_context_base(execution_context_base&& other);
	void notify_recipients(bool i_useAndKeep);
	bool transfer_recipients(execution_context_base&& other);
	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE(Callable))
	continuation_token enqueue(Callable&& i_callable, unsigned char i_depth);
	bool transfer(execution_context_base&& other);
	bool dismiss(unsigned char i_depth,continuation_token i_token);

	execution_context_base& operator=(execution_context_base&&) = default;

private:
	detail::async_executor_recipients m_recipients;
	execution_recipients_ref m_recipientsRef;
};

}

typedef lent_reference_wrapper<detail::execution_context_base> executor_context_lent_ref;
typedef lent_reference_wrapper<const detail::execution_context_base> executor_context_const_lent_ref;
typedef lent_pointer_wrapper<detail::execution_context_base> executor_context_lent_ptr;
typedef lent_pointer_wrapper<const detail::execution_context_base> executor_context_const_lent_ptr;

class immediate_execution_context : public detail::execution_context_base, public lend_from_this<immediate_execution_context,detail::execution_context_base>
{
public:
	using detail::execution_context_base::enqueue;

	TEMPLATE(typename Callable)
	REQUIRES(IS_CALLABLE(Callable))
	void start(Callable&& i_callable, bool i_useAndKeep);
	bool cancel();
};

class thread_execution_context : public detail::execution_context_base, public lend_from_this<thread_execution_context,detail::execution_context_base>
{
public:
	thread_execution_context(thread i_thread);
	void start(function<void()>, bool i_useAndKeep);
	bool cancel();

private:
	thread m_thread;
};

class fiber_execution_context : public detail::execution_context_base, public lend_from_this<fiber_execution_context,detail::execution_context_base>
{
public:
	fiber_execution_context(fiber i_fiber);

	void start(function<void()>, bool i_useAndKeep);
	bool cancel();

private:
	fiber m_fiber;
};

class thread_sheaf_execution_context : public detail::execution_context_base, public lend_from_this<thread_sheaf_execution_context,detail::execution_context_base>
{
public:
	thread_sheaf_execution_context(thread_sheaf i_threadSheaf);
	bool cancel();
	size_t add_failure();
	size_t remove_pending_fiber();
	bool has_pending_threads() const;
	bool has_failures() const;
	continuation_token enqueue(const function<void()>&);

	void start(function<void()>);

private:
	thread_sheaf m_threadSheaf;
	atomic_size_t m_failedThreads;
	atomic_size_t m_pendingThreads;
};

class fiber_sheaf_execution_context : public detail::execution_context_base, public lend_from_this<fiber_sheaf_execution_context,detail::execution_context_base>
{
public:
	fiber_sheaf_execution_context(fiber_sheaf i_fiberSheaf);
	~fiber_sheaf_execution_context();

	bool cancel();
	void clear_fibers();
	size_t add_success();
	size_t remove_pending_thread();
	bool has_pending_fibers() const;
	bool has_succeed() const;
	continuation_token enqueue(const function<void()>&);

	void start(function<void()>);

private:
	fiber_sheaf m_fiberSheaf;
	atomic_size_t m_successFibers;
	atomic_size_t m_pendingFibers;
};

}

#include "ddk_sync_executor_context.inl"