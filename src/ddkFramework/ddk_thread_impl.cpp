#include "ddk_thread_impl.h"
#include "ddk_reference_wrapper.h"
#include "ddk_fiber_impl.h"

namespace ddk
{
namespace
{

thread_local ddk::detail::this_thread_t* g_this_thread = nullptr;

bool initialize_this_thread()
{
    static thread_local ddk::detail::this_thread_t s_this_thread;

    g_this_thread = &s_this_thread;

    return g_this_thread != nullptr;
}

const bool g_mainThreadInitialization = initialize_this_thread();

}

namespace detail
{

yielder* thread_impl_interface::get_yielder()
{
	static threadlocal<yielder*,thread_impl_interface> s_yielder;

	return s_yielder.get();
}
yielder* thread_impl_interface::set_yielder(yielder* i_yielder)
{
	static threadlocal<yielder*,thread_impl_interface> s_yielder;

	yielder* prevYielder = (s_yielder.empty() == false) ? std::move(s_yielder).extract() : nullptr;

	s_yielder.set(i_yielder);

	return prevYielder;
}
void thread_impl_interface::clear_yielder()
{
	static threadlocal<yielder*,thread_impl_interface> yielder;

	yielder.clear();
}

void* threadFunc(void *ptr)
{
	if(thread_impl_interface* thread = reinterpret_cast<thread_impl_interface*>(ptr))
	{
        initialize_this_thread();

		thread->execute();
	}

	return nullptr;
}

one_shot_thread_impl::one_shot_thread_impl()
{
}
one_shot_thread_impl::~one_shot_thread_impl()
{
	stop().dismiss();
}
typename one_shot_thread_impl::start_result one_shot_thread_impl::start(const ddk::function<void()>& i_function, yielder* i_yielder)
{
	if(m_started == false)
	{
		if(i_function != nullptr)
		{
			m_started = true;
			m_threadFunc = i_function;
			pthread_attr_t	attr;
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

			m_yielder = i_yielder;

			pthread_create(&m_thread, &attr, threadFunc, this);

			pthread_attr_destroy(&attr);

			return success;
		}
		else
		{
			return ddk::make_error<start_result>(StartErrorCode::StartNoCallable,"Provided callable is empty.");
		}
	}
	else
	{
		return ddk::make_error<start_result>(StartErrorCode::StartNotAvailable, "Thread is alreday started.");
	}
}
typename one_shot_thread_impl::stop_result one_shot_thread_impl::stop()
{
	if(m_started)
	{
		void *res = nullptr;
		const int joinRes = pthread_join(m_thread, &res);

		if (joinRes == 0)
		{
			m_started = false;

			return success;
		}
		else
		{
			return ddk::make_error<stop_result>(StopErrorCode::NotStoppable, "Could not stop thread: " + ddk::to_string(joinRes));
		}
	}
	else
	{
		return ddk::make_error<stop_result>(StopErrorCode::NotRunning,"Trying to stop not running thread.");
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
	if(i_exiting)
	{
		m_started = false;

		thread_impl_interface::clear_yielder();

		m_yielder = nullptr;
	}
}
void one_shot_thread_impl::execute()
{
	if(m_threadFunc != nullptr)
	{
		thread_impl_interface::set_yielder(m_yielder);

		eval(m_threadFunc);

		thread_impl_interface::clear_yielder();

		m_yielder = nullptr;

		m_threadFunc = nullptr;
	}
}
bool one_shot_thread_impl::set_affinity(const cpu_set_t& i_set)
{
	return pthread_setaffinity_np(m_thread,sizeof(cpu_set_t),&i_set) == 0;
}

}

const detail::this_thread_t& this_thread()
{
    return *g_this_thread;
}

}
