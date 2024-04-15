#include "ddk_sync_executor_context.h"
#include "ddk_lock_guard.h"

namespace ddk
{
namespace detail
{

async_executor_recipients::task::task(size_t i_token, function<void()> i_callable)
: m_token(i_token)
, m_callable(std::move(i_callable))
{
}
bool async_executor_recipients::task::operator==(const continuation_token& i_token) const
{
	return i_token == m_token;
}

async_executor_recipients::async_executor_recipients()
: m_mutex(MutexType::Recursive)
{
}
async_executor_recipients::async_executor_recipients(async_executor_recipients&& other)
: m_mutex(MutexType::Recursive)
, m_pendingCallables(std::move(other.m_pendingCallables))
, m_admissible(other.m_admissible)
{
	other.m_admissible = false;
}
async_executor_recipients& async_executor_recipients::operator=(async_executor_recipients&& other)
{
	m_pendingCallables = std::move(other.m_pendingCallables);

	m_admissible = std::move(other.m_admissible);
	other.m_admissible = false;

	return *this;
}
void async_executor_recipients::notify(bool i_useAndKeep)
{
	mutex_guard mg(m_mutex);

	m_admissible = false;

	if (i_useAndKeep)
	{
		callable_container pendingCallables = m_pendingCallables;

		resolve(pendingCallables);
	}
	else
	{
		resolve(m_pendingCallables);
	}
}
bool async_executor_recipients::dismiss(unsigned char i_depth, continuation_token i_token)
{
	mutex_guard mg(m_mutex);

	std::list<task>& groupedCallables = m_pendingCallables[i_depth];
	std::list<task>::iterator itTask = std::find(groupedCallables.begin(),groupedCallables.end(),i_token);

	if(itTask != groupedCallables.end())
	{
		groupedCallables.erase(itTask);

		return true;
	}
	else
	{
		return false;
	}
}
bool async_executor_recipients::move(async_executor_recipients&& i_recipients)
{
	mutex_guard mg(m_mutex);

	if (is_admissible())
	{
		callable_container pendingCallables = std::move(i_recipients.m_pendingCallables);

		for (auto&& valuePair : pendingCallables)
		{
			std::list<task>& taskList = m_pendingCallables[valuePair.first];
			std::list<task>& otherTaskList = valuePair.second;

			taskList.insert(taskList.end(),otherTaskList.begin(),otherTaskList.end());
		}

		return true;
	}
	else
	{
		return false;
	}
}
void async_executor_recipients::clear()
{
	mutex_guard mg(m_mutex);

	m_pendingCallables.clear();
}
bool async_executor_recipients::is_admissible()
{
	return (call_admissionPredicate != nullptr) ? eval(call_admissionPredicate,m_admissible) : m_admissible;
}
void async_executor_recipients::resolve(callable_container& i_pendingCallables)
{
	callable_container::iterator itGroupedMaps = i_pendingCallables.begin();

	while (itGroupedMaps != i_pendingCallables.end())
	{
		std::list<task> groupedMaps = std::move(itGroupedMaps->second);

		i_pendingCallables.erase(itGroupedMaps);

		while (groupedMaps.empty() == false)
		{
			const task _task = groupedMaps.front();

			groupedMaps.pop_front();

			m_mutex.unlock();

			eval(_task);

			m_mutex.lock();
		}

		itGroupedMaps = i_pendingCallables.begin();
	}
}

execution_context_base::execution_context_base()
: m_recipientsRef(lend(m_recipients))
{
}
execution_context_base::execution_context_base(execution_context_base&& other)
: m_recipients(std::move(other.m_recipients))
, m_recipientsRef(lend(m_recipients))
{
}
void execution_context_base::notify_recipients(bool i_useAndKeep)
{
	m_recipientsRef->notify(i_useAndKeep);
}
bool execution_context_base::transfer_recipients(execution_context_base&& other)
{
	return m_recipientsRef->move(std::move(other.m_recipients));
}
bool execution_context_base::transfer(execution_context_base&& other)
{
	other.enqueue(make_function([&]()
	{
		other.m_recipients.call_admissionPredicate = nullptr;
		other.m_recipientsRef = lend(other.m_recipients);
	}),0);

	if (transfer_recipients(std::move(other)))
	{
		other.m_recipientsRef = lend(m_recipients);

		return true;
	}
	else
	{
		return false;
	}
}
bool execution_context_base::dismiss(unsigned char i_depth,continuation_token i_token)
{
	return m_recipientsRef->dismiss(i_depth,std::move(i_token));
}
void execution_context_base::admission_predicate(const function<bool(bool)>& i_callable)
{
	m_recipientsRef->call_admissionPredicate = i_callable;
}

}

continuation_token::continuation_token(size_t i_token)
: m_id(i_token)
{
}
continuation_token::continuation_token(continuation_token&& other)
: m_id(0)
{
	std::swap(m_id,other.m_id);
}
continuation_token& continuation_token::operator=(continuation_token&& other)
{
	m_id = ntoken;

	std::swap(m_id,other.m_id);

	return *this;
}
bool continuation_token::operator==(const continuation_token& other) const
{
	return m_id == other.m_id;
}
bool continuation_token::operator==(const size_t& other) const
{
	return m_id == other;
}
continuation_token::operator bool() const
{
	return m_id != ntoken;
}

bool immediate_execution_context::cancel()
{
	return true;
}

thread_execution_context::thread_execution_context(thread i_thread)
: m_thread(std::move(i_thread))
{
}
void thread_execution_context::start(function<void()> i_callable, bool i_useAndKeep)
{
	m_thread.start([this,callable=std::move(i_callable),useAndKeep = i_useAndKeep]() mutable
	{
		eval(callable);

		notify_recipients(useAndKeep);
	}).dismiss();
}
bool thread_execution_context::cancel()
{
	notify_recipients(false);

	if(m_thread.joinable() == false)
	{
		return true;
	}
	else
	{
		return false;
	}
}

fiber_execution_context::fiber_execution_context(fiber i_fiber)
: m_fiber(std::move(i_fiber))
{
}
void fiber_execution_context::start(function<void()> i_callable, bool i_useAndKeep)
{
	m_fiber.start([=]()
	{
		eval(i_callable);

		notify_recipients(i_useAndKeep);
	}).dismiss();
}
bool fiber_execution_context::cancel()
{
	if(m_fiber.joinable() == false)
	{
		notify_recipients(false);

		return true;
	}
	else
	{
		return false;
	}
}

thread_sheaf_execution_context::thread_sheaf_execution_context(thread_sheaf i_threadSheaf)
: m_threadSheaf(std::move(i_threadSheaf))
, m_failedThreads(0)
, m_pendingThreads(m_threadSheaf.size())
{
}
void thread_sheaf_execution_context::start(function<void()> i_callable)
{
	m_threadSheaf.start([=]()
	{
		eval(i_callable);
	}).dismiss();
}
bool thread_sheaf_execution_context::cancel()
{
	if(m_threadSheaf.joinable() == false)
	{
		notify_recipients(false);

		return true;
	}
	else
	{
		return false;
	}
}
continuation_token thread_sheaf_execution_context::enqueue(const function<void()>& i_callable)
{
	return execution_context_base::enqueue(i_callable,0);
}
size_t thread_sheaf_execution_context::add_failure()
{
	return atomic_post_increment(m_failedThreads);
}
size_t thread_sheaf_execution_context::remove_pending_fiber()
{
	return atomic_post_decrement(m_pendingThreads);
}
bool thread_sheaf_execution_context::has_pending_threads() const
{
	return m_pendingThreads.get() > 0;
}
bool thread_sheaf_execution_context::has_failures() const
{
	return m_failedThreads.get() > 0;
}

fiber_sheaf_execution_context::fiber_sheaf_execution_context(fiber_sheaf i_fiberSheaf)
: m_fiberSheaf(std::move(i_fiberSheaf))
, m_successFibers(0)
, m_pendingFibers(m_fiberSheaf.size())
{
}
fiber_sheaf_execution_context::~fiber_sheaf_execution_context()
{
	int a = 0;
	++a;
}
void fiber_sheaf_execution_context::start(function<void()> i_callable)
{
	m_fiberSheaf.start([=]()
	{
		eval(i_callable);
	}).dismiss();
}
bool fiber_sheaf_execution_context::cancel()
{
	if(m_fiberSheaf.joinable() == false)
	{
		notify_recipients(false);

		return true;
	}
	else
	{
		return false;
	}
}
continuation_token fiber_sheaf_execution_context::enqueue(const function<void()>& i_callable)
{
	return execution_context_base::enqueue(i_callable,0);
}
void fiber_sheaf_execution_context::clear_fibers()
{
	m_fiberSheaf.clear();
}
size_t fiber_sheaf_execution_context::add_success()
{
	return atomic_post_increment(m_successFibers);
}
size_t fiber_sheaf_execution_context::remove_pending_thread()
{
	return atomic_post_decrement(m_pendingFibers);
}
bool fiber_sheaf_execution_context::has_pending_fibers() const
{
	return m_pendingFibers.get() > 0;
}
bool fiber_sheaf_execution_context::has_succeed() const
{
	return m_successFibers.get() == m_fiberSheaf.size();
}

}