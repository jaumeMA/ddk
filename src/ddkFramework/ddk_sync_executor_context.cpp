#include "ddk_sync_executor_context.h"
#include "ddk_lock_guard.h"

namespace ddk
{

void deferred_execution_context::start(const function<void()>& i_callable)
{
	eval(i_callable);
}
bool deferred_execution_context::enqueue(const function<void()>& i_callable)
{
	eval(i_callable);

	return true;
}
void deferred_execution_context::clear()
{
}

thread_execution_context::thread_execution_context(thread i_thread)
: m_thread(std::move(i_thread))
, m_alive(true)
{
}
void thread_execution_context::start(const function<void()>& i_callable)
{
	m_thread.start([=]()
	{
		eval(i_callable);

		m_mutex.lock();

		while(m_pendingCallables.empty() == false)
		{
			const function<void()> task = m_pendingCallables.front();

			m_pendingCallables.pop();

			m_mutex.unlock();

			eval(task);

			m_mutex.lock();
		}

		m_alive = false;

		m_mutex.unlock();
	});
}
bool thread_execution_context::enqueue(const function<void()>& i_callable)
{
	{
		mutex_guard mg(m_mutex);

		if(m_alive)
		{
			m_pendingCallables.push(i_callable);

			return true;
		}
	}

	eval(i_callable);

	return false;
}
void thread_execution_context::clear()
{
	mutex_guard mg(m_mutex);

	while(m_pendingCallables.empty() == false) m_pendingCallables.pop();
}

fiber_execution_context::fiber_execution_context(fiber i_fiber)
: m_fiber(std::move(i_fiber))
, m_alive(true)
{
}
void fiber_execution_context::start(const function<void()>& i_callable)
{
	m_fiber.start([=]()
	{
		eval(i_callable);

		m_mutex.lock();

		while(m_pendingCallables.empty() == false)
		{
			const function<void()> task = m_pendingCallables.front();

			m_pendingCallables.pop();

			m_mutex.unlock();

			eval(task);

			m_mutex.lock();
		}

		m_alive = false;

		m_mutex.unlock();
	});
}
bool fiber_execution_context::enqueue(const function<void()>& i_callable)
{
	{
		mutex_guard mg(m_mutex);

		if(m_alive)
		{
			m_pendingCallables.push(i_callable);

			return true;
		}
	}

	eval(i_callable);

	return false;
}
void fiber_execution_context::clear()
{
	mutex_guard mg(m_mutex);

	while(m_pendingCallables.empty() == false) m_pendingCallables.pop();
}

thread_sheaf_execution_context::thread_sheaf_execution_context(thread_sheaf i_threadSheaf)
: m_threadSheaf(std::move(i_threadSheaf))
, m_failedThreads(0)
, m_pendingThreads(m_threadSheaf.size())
, m_alive(true)
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
			m_mutex.lock();

			while(m_pendingCallables.empty() == false)
			{
				const function<void()> task = m_pendingCallables.front();

				m_pendingCallables.pop();

				m_mutex.unlock();

				eval(task);

				m_mutex.lock();
			}

			m_alive = false;

			m_mutex.unlock();
		}
	});
}
bool thread_sheaf_execution_context::enqueue(const function<void()>& i_callable)
{
	{
		mutex_guard mg(m_mutex);

		if(m_alive)
		{
			m_pendingCallables.push(i_callable);

			return true;
		}
	}

	eval(i_callable);

	return false;
}
void thread_sheaf_execution_context::clear()
{
	mutex_guard mg(m_mutex);

	while(m_pendingCallables.empty() == false) m_pendingCallables.pop();
}
size_t thread_sheaf_execution_context::get_num_failures() const
{
	return m_failedThreads.get();
}

fiber_sheaf_execution_context::fiber_sheaf_execution_context(fiber_sheaf i_fiberSheaf)
: m_fiberSheaf(std::move(i_fiberSheaf))
, m_failedThreads(0)
, m_pendingThreads(m_fiberSheaf.size())
, m_alive(true)
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
			m_mutex.lock();

			while(m_pendingCallables.empty() == false)
			{
				const function<void()> task = m_pendingCallables.front();

				m_pendingCallables.pop();

				m_mutex.unlock();

				eval(task);

				m_mutex.lock();
			}

			m_alive = false;

			m_mutex.unlock();
		}
	});
}
bool fiber_sheaf_execution_context::enqueue(const function<void()>& i_callable)
{
	{
		mutex_guard mg(m_mutex);

		if(m_alive)
		{
			m_pendingCallables.push(i_callable);

			return true;
		}
	}

	eval(i_callable);

	return false;
}
void fiber_sheaf_execution_context::clear()
{
	mutex_guard mg(m_mutex);

	while(m_pendingCallables.empty() == false) m_pendingCallables.pop();
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