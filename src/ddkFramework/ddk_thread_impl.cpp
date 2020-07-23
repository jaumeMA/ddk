#include "ddk_thread_impl.h"
#include "ddk_reference_wrapper.h"
#include "ddk_fiber_impl.h"

namespace ddk
{
namespace detail
{

threadlocal<yielder_lent_ptr,thread_impl_interface>& thread_impl_interface::get_yielder_local()
{
	static threadlocal<yielder_lent_ptr,thread_impl_interface> s_yielder;

	return s_yielder;
}
yielder_lent_ptr thread_impl_interface::get_yielder()
{
	threadlocal<yielder_lent_ptr,thread_impl_interface>& yielder = get_yielder_local();

	return yielder.get();
}
yielder_lent_ptr thread_impl_interface::set_yielder(yielder_lent_ptr i_yielder)
{
	threadlocal<yielder_lent_ptr,thread_impl_interface>& yielder = get_yielder_local();

	yielder_lent_ptr prevYielder = (yielder.empty() == false) ? yielder.extract() : nullptr;

	yielder.set(i_yielder);

	return prevYielder;
}
void thread_impl_interface::clear_yielder()
{
	threadlocal<yielder_lent_ptr,thread_impl_interface>& yielder = get_yielder_local();

	yielder.clear();	
}

void* threadFunc(void *ptr)
{
	if(thread_impl_interface* thread = reinterpret_cast<thread_impl_interface*>(ptr))
	{
		thread->execute();
	}

	return NULL;
}
void threadExiting(void* ptr)
{
	if(thread_impl_interface* thread = reinterpret_cast<thread_impl_interface*>(ptr))
	{
		thread->setExiting(true);
	}
}

void one_shot_thread_impl::start(const std::function<void()>& i_function, yielder_lent_ptr i_yielder)
{
	if(m_started == false)
	{
		m_threadFunc = i_function;

		pthread_attr_t	attr;
		pthread_attr_init(&attr);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		m_yielder = i_yielder;

		pthread_create(&m_thread, &attr, threadFunc, this);

		pthread_attr_destroy(&attr);

		m_started = true;
	}
}
void one_shot_thread_impl::stop()
{
	if(m_started)
	{
		m_started = false;
		m_threadFunc = nullptr;

		void *res = NULL;
		pthread_join(m_thread,&res);
	}
}
bool one_shot_thread_impl::joinable() const
{
	return m_started;
}
thread_id_t one_shot_thread_impl::get_id() const
{
  return get_thread_id(m_thread);
}
void one_shot_thread_impl::setExiting(bool i_exiting)
{
	m_started = !i_exiting;
}
void one_shot_thread_impl::execute()
{
	if(m_threadFunc)
	{
		thread_impl_interface::set_yielder(m_yielder);

		pthread_cleanup_push(&threadExiting,this);

		m_threadFunc();

		thread_impl_interface::clear_yielder();

		m_yielder = nullptr;

		pthread_cleanup_pop(0);
	}
}

this_thread_t::this_thread_t()
: m_thread(get_current_thread())
{
}
this_thread_t::id this_thread_t::get_id() const
{
	return get_current_thread_id();
}

}
}
