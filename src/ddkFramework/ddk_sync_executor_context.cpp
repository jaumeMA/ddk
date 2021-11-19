#include "ddk_sync_executor_context.h"
#include "ddk_lock_guard.h"

namespace ddk
{
namespace detail
{

async_executor_recipients::task::task(size_t i_token,const function<void()>& i_callable)
: m_token(i_token)
, m_callable(i_callable)
{
}
bool async_executor_recipients::task::operator==(const continuation_token& i_token) const
{
	return i_token == m_token;
}

async_executor_recipients::~async_executor_recipients()
{
	mutex_guard mg(m_mutex);
}
void async_executor_recipients::notify()
{
	m_mutex.lock();

	callable_container::iterator itGroupedMaps = m_pendingCallables.begin();

	while(itGroupedMaps != m_pendingCallables.end())
	{
		std::list<task>& groupedMaps = itGroupedMaps->second;

		while(groupedMaps.empty() == false)
		{
			const task _task = groupedMaps.front();

			groupedMaps.pop_front();

			m_mutex.unlock();

			eval(_task);

			m_mutex.lock();
		}

		itGroupedMaps = m_pendingCallables.erase(itGroupedMaps);
	}

	m_admissible = false;

	m_mutex.unlock();
}
continuation_token async_executor_recipients::accept(const function<void()>& i_callable, unsigned char i_depth)
{
	mutex_guard mg(m_mutex);

	if(m_admissible)
	{
		const size_t token = std::rand();

		m_pendingCallables[i_depth].emplace_back(token,i_callable);

		return {token};
	}
	else
	{
		return continuation_token::ntoken;
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
void async_executor_recipients::clear()
{
	mutex_guard mg(m_mutex);

	m_pendingCallables.clear();
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

void deferred_execution_context::start(const function<void()>& i_callable)
{
	eval(i_callable);
}
bool deferred_execution_context::cancel()
{
	return true;
}
continuation_token deferred_execution_context::enqueue(const function<void()>& i_callable, unsigned char i_depth)
{
	return { continuation_token::ntoken };
}
bool deferred_execution_context::dismiss(unsigned char i_depth,continuation_token i_token)
{
	return false;
}
void deferred_execution_context::clear()
{
}

thread_execution_context::thread_execution_context(thread i_thread)
: m_thread(std::move(i_thread))
{
}
bool thread_execution_context::cancel()
{
	if(m_thread.joinable() == false)
	{
		m_recipients.notify();

		return true;
	}
	else
	{
		return false;
	}
}
void thread_execution_context::start(const function<void()>& i_callable)
{
	m_thread.start([=]()
	{
		eval(i_callable);

		m_recipients.notify();
	});
}
continuation_token thread_execution_context::enqueue(const function<void()>& i_callable, unsigned char i_depth)
{
	return m_recipients.accept(i_callable,i_depth);
}
bool thread_execution_context::dismiss(unsigned char i_depth,continuation_token i_token)
{
	return m_recipients.dismiss(i_depth,std::move(i_token));
}
void thread_execution_context::clear()
{
	m_recipients.clear();
}

fiber_execution_context::fiber_execution_context(fiber i_fiber)
: m_fiber(std::move(i_fiber))
{
}
void fiber_execution_context::start(const function<void()>& i_callable)
{
	m_fiber.start([=]()
	{
		eval(i_callable);

		m_recipients.notify();
	});
}
bool fiber_execution_context::cancel()
{
	if(m_fiber.joinable() == false)
	{
		m_recipients.notify();

		return true;
	}
	else
	{
		return false;
	}
}
continuation_token fiber_execution_context::enqueue(const function<void()>& i_callable, unsigned char i_depth)
{
	return m_recipients.accept(i_callable,i_depth);
}
bool fiber_execution_context::dismiss(unsigned char i_depth,continuation_token i_token)
{
	return m_recipients.dismiss(i_depth,std::move(i_token));
}
void fiber_execution_context::clear()
{
	m_recipients.clear();
}

thread_sheaf_execution_context::thread_sheaf_execution_context(thread_sheaf i_threadSheaf)
: m_threadSheaf(std::move(i_threadSheaf))
, m_failedThreads(0)
, m_pendingThreads(m_threadSheaf.size())
{
}
bool thread_sheaf_execution_context::cancel()
{
	if(m_threadSheaf.joinable() == false)
	{
		m_recipients.notify();

		return true;
	}
	else
	{
		return false;
	}
}
void thread_sheaf_execution_context::start(const function<void()>& i_callable)
{
	m_threadSheaf.start([i_callable]()
	{
		eval(i_callable);
	});
}
continuation_token thread_sheaf_execution_context::enqueue(const function<void()>& i_callable)
{
	return enqueue(i_callable,0);
}
continuation_token thread_sheaf_execution_context::enqueue(const function<void()>& i_callable, unsigned char i_depth)
{
	return m_recipients.accept(i_callable,i_depth);
}
bool thread_sheaf_execution_context::dismiss(unsigned char i_depth,continuation_token i_token)
{
	return m_recipients.dismiss(i_depth,std::move(i_token));
}
void thread_sheaf_execution_context::clear()
{
	m_recipients.clear();
}
void thread_sheaf_execution_context::notify_recipients()
{
	m_recipients.notify();
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
, m_failedFibers(0)
, m_pendingFibers(m_fiberSheaf.size())
{
}
void fiber_sheaf_execution_context::start(const function<void()>& i_callable)
{
	m_fiberSheaf.start([i_callable]()
	{
		eval(i_callable);
	});
}
bool fiber_sheaf_execution_context::cancel()
{
	if(m_fiberSheaf.joinable() == false)
	{
		m_recipients.notify();

		return true;
	}
	else
	{
		return false;
	}
}
continuation_token fiber_sheaf_execution_context::enqueue(const function<void()>& i_callable)
{
	return enqueue(i_callable,0);
}
continuation_token fiber_sheaf_execution_context::enqueue(const function<void()>& i_callable, unsigned char i_depth)
{
	return m_recipients.accept(i_callable,i_depth);
}
bool fiber_sheaf_execution_context::dismiss(unsigned char i_depth,continuation_token i_token)
{
	return m_recipients.dismiss(i_depth,std::move(i_token));
}
void fiber_sheaf_execution_context::clear()
{
	m_recipients.clear();
}
void fiber_sheaf_execution_context::clear_fibers()
{
	m_fiberSheaf.clear();
}
void fiber_sheaf_execution_context::notify_recipients()
{
	m_recipients.notify();
}
size_t fiber_sheaf_execution_context::add_failure()
{
	return atomic_post_increment(m_failedFibers);
}
size_t fiber_sheaf_execution_context::remove_pending_thread()
{
	return atomic_post_decrement(m_pendingFibers);
}
bool fiber_sheaf_execution_context::has_pending_fibers() const
{
	return m_pendingFibers.get() > 0;
}
bool fiber_sheaf_execution_context::has_failures() const
{
	return m_failedFibers.get() > 0;
}

}