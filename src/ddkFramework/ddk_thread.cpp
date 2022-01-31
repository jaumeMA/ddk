#include "ddk_thread.h"
#include "ddk_thread_utils.h"
#include "ddk_reference_wrapper.h"

namespace ddk
{

thread::thread()
: m_threadImpl(make_unique_reference<detail::one_shot_thread_impl>())
{
}
thread::thread(detail::thread_impl_unique_ref i_threadImpl)
: m_threadImpl(std::move(i_threadImpl))
{
}
thread::thread(thread&& other)
: m_threadImpl(std::move(other.m_threadImpl))
{
}
thread::~thread()
{
	stop().dismiss();
}
thread& thread::operator=(thread&& other)
{
	if(m_threadImpl.get() != nullptr)
	{
		m_threadImpl->stop();
	}

	m_threadImpl = std::move(other.m_threadImpl);

	return *this;
}
thread::start_result thread::start(const ddk::function<void()>& i_threadFunc, detail::yielder* i_yielder)
{
	if(m_threadImpl.get() != nullptr)
	{
		return m_threadImpl->start(i_threadFunc,i_yielder);
	}
	else
	{
		return make_error<start_result>(detail::thread_impl_interface::StartErrorCode::StartNotAvailable);
	}
}
thread::stop_result thread::stop()
{
	if(m_threadImpl.get() != nullptr)
	{
		return m_threadImpl->stop();
	}
	else
	{
		return make_error<stop_result>(detail::thread_impl_interface::StopErrorCode::NotStoppable);
	}
}
thread::id thread::get_id() const
{
	return m_threadImpl->get_id();
}
bool thread::set_affinity(const cpu_set_t& i_set)
{
	return (m_threadImpl.get() != nullptr) ? m_threadImpl->set_affinity(i_set) : false;
}
bool thread::joinable() const
{
	return (m_threadImpl.get() != nullptr) ? m_threadImpl->joinable() : false;
}

}
