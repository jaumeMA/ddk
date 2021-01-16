#include "ddk_watch_dog.h"
#include "ddk_thread_utils.h"
#include "ddk_lock_guard.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{
namespace detail
{

void watch_dog_waiter::lock()
{
	m_mutex.lock();
}
void watch_dog_waiter::unlock()
{
	m_mutex.unlock();
}
void watch_dog_waiter::wait()
{
	mutex_guard lg(m_mutex);

	m_condVar.wait(m_mutex);
}
void watch_dog_waiter::wait_until(const function<bool()>& i_predicate)
{
	mutex_guard lg(m_mutex);

	m_condVar.wait_until(m_mutex,i_predicate);
}
void watch_dog_waiter::notify_one()
{
	m_condVar.notify_one();
}
void watch_dog_waiter::notify_all()
{
	m_condVar.notify_all();
}

}

watch_dog::watch_dog(watch_dog_waiter_weak_ptr& i_weakWaiter)
{
	if(watch_dog_waiter_shared_ptr waiterSharedPtr = share(i_weakWaiter))
	{
		m_waiter = waiterSharedPtr;
	}
	else
	{
		watch_dog_waiter_shared_ptr newWaiter = make_atomic_shared_reference<detail::watch_dog_waiter>();

		m_waiter = share(i_weakWaiter = weak(newWaiter));
	}
}
void watch_dog::watch()
{
	if(m_waiter)
	{
		m_waiter->wait();
	}
}
void watch_dog::watch_until(const function<bool()>& i_predicate)
{
	if(m_waiter)
	{
		m_waiter->wait_until(i_predicate);
	}
}
void watch_dog::notify_one()
{
	if(m_waiter)
	{
		m_waiter->notify_one();
	}
}
void watch_dog::notify_all()
{
	if(m_waiter)
	{
		m_waiter->notify_all();
	}
}

}
