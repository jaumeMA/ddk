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
void watch_dog_waiter::wait(const function<bool()>& i_predicate)
{
	lock_guard lg(m_mutex);

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
	if(watch_dog_waiter_shared_ptr waiterSharedPtr = i_weakWaiter.share())
	{
		m_waiter = waiterSharedPtr;
	}
	else
	{
		m_waiter = make_shared_reference<detail::watch_dog_waiter>();

		i_weakWaiter = weak(m_waiter);
	}
}
void watch_dog::wait_until(const function<bool()>& i_predicate)
{
	if(m_waiter)
	{
		m_waiter->wait(i_predicate);
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