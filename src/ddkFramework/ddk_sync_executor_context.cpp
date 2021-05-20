#include "ddk_sync_executor_context.h"
#include "ddk_lock_guard.h"

namespace ddk
{
namespace detail
{

void async_executor_recipients::notify()
{
	m_mutex.lock();

	while(m_pendingCallables.empty() == false)
	{
		std::map<unsigned char,std::queue<function<void()>>>::iterator itGroupedMaps = m_pendingCallables.begin();
		std::queue<function<void()>>& groupedMaps = itGroupedMaps->second;

		while(groupedMaps.empty() == false)
		{
			const function<void()> task = groupedMaps.front();

			groupedMaps.pop();

			m_mutex.unlock();

			eval(task);

			m_mutex.lock();
		}

		m_pendingCallables.erase(itGroupedMaps);
	}

	m_admissible = false;

	m_mutex.unlock();
}
bool async_executor_recipients::accept(const function<void()>& i_callable, unsigned char i_depth)
{
	mutex_guard mg(m_mutex);

	if(m_admissible)
	{
		m_pendingCallables[i_depth].push(i_callable);

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


void deferred_execution_context::start(const function<void()>& i_callable)
{
	eval(i_callable);
}
bool deferred_execution_context::enqueue(const function<void()>& i_callable, unsigned char i_depth)
{
	eval(i_callable);

	return true;
}
void deferred_execution_context::clear()
{
}

thread_execution_context::thread_execution_context(thread i_thread)
: m_thread(std::move(i_thread))
{
}
void thread_execution_context::start(const function<void()>& i_callable)
{
	m_thread.start([=]()
	{
		eval(i_callable);

		m_recipients.notify();
	});
}
bool thread_execution_context::enqueue(const function<void()>& i_callable, unsigned char i_depth)
{
	if(m_recipients.accept(i_callable,i_depth) == false)
	{
		eval(i_callable);

		return false;
	}

	return true;
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
bool fiber_execution_context::enqueue(const function<void()>& i_callable, unsigned char i_depth)
{
	if(m_recipients.accept(i_callable,i_depth) == false)
	{
		eval(i_callable);

		return false;
	}

	return true;
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
void thread_sheaf_execution_context::start(const function<void()>& i_callable)
{
	m_threadSheaf.start([=]()
	{
		try
		{
			eval(i_callable);
		}
		catch(...)
		{
			atomic_post_increment(m_failedThreads);
		}

		atomic_post_decrement(m_pendingThreads);

		if(m_pendingThreads.get() == 0)
		{
			m_recipients.notify();
		}
	});
}
bool thread_sheaf_execution_context::enqueue(const function<void()>& i_callable)
{
	return enqueue(i_callable,0);
}
bool thread_sheaf_execution_context::enqueue(const function<void()>& i_callable, unsigned char i_depth)
{
	if(m_recipients.accept(i_callable,i_depth) == false)
	{
		eval(i_callable);

		return false;
	}

	return true;
}
void thread_sheaf_execution_context::clear()
{
	m_recipients.clear();
}
size_t thread_sheaf_execution_context::get_num_failures() const
{
	return m_failedThreads.get();
}

fiber_sheaf_execution_context::fiber_sheaf_execution_context(fiber_sheaf i_fiberSheaf)
: m_fiberSheaf(std::move(i_fiberSheaf))
, m_failedThreads(0)
, m_pendingThreads(m_fiberSheaf.size())
{
}
void fiber_sheaf_execution_context::start(const function<void()>& i_callable)
{
	m_fiberSheaf.start([=]()
	{
		try
		{
			eval(i_callable);
		}
		catch(...)
		{
			atomic_post_increment(m_failedThreads);
		}

		atomic_post_decrement(m_pendingThreads);

		if(m_pendingThreads.get() == 0)
		{
			m_recipients.notify();
		}
	});
}
bool fiber_sheaf_execution_context::enqueue(const function<void()>& i_callable)
{
	return enqueue(i_callable,0);
}
bool fiber_sheaf_execution_context::enqueue(const function<void()>& i_callable, unsigned char i_depth)
{
	if(m_recipients.accept(i_callable,i_depth) == false)
	{
		eval(i_callable);

		return false;
	}

	return true;
}
void fiber_sheaf_execution_context::clear()
{
	m_recipients.clear();
}
void fiber_sheaf_execution_context::clear_fibers()
{
	m_fiberSheaf.clear();
}
size_t fiber_sheaf_execution_context::get_num_failures() const
{
	return m_failedThreads.get();
}

}