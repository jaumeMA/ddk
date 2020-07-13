#include "ddk_thread.h"
#include "thread_utils.h"
#include "reference_wrapper.h"

namespace ddk
{

const detail::this_thread_t this_thread = detail::this_thread_t();

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
	stop();
}
thread& thread::operator=(thread&& other)
{
	if(m_threadImpl->joinable())
	{
		m_threadImpl->stop();
	}

	m_threadImpl = std::move(other.m_threadImpl);

	return *this;
}
void thread::start(const std::function<void()>& i_threadFunc, detail::yielder_lent_ptr i_yielder)
{
	m_threadImpl->start(i_threadFunc,i_yielder);
}
thread::id thread::get_id() const
{
	return m_threadImpl->get_id();
}
void thread::stop()
{
	m_threadImpl->stop();
}
bool thread::joinable() const
{
	return m_threadImpl->joinable();
}

}
