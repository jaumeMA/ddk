#include "ddk_fiber.h"

namespace ddk
{

fiber::fiber(detail::fiber_impl_unique_ref i_fiber)
: m_impl(std::move(i_fiber))
{
}
fiber::fiber(fiber&& other)
: m_impl(std::move(other.m_impl))
{
}
fiber& fiber::operator=(fiber&& other)
{
	m_impl = std::move(other.m_impl);

	return *this;
}
fiber::start_result fiber::start(const ddk::function<void()>& i_function)
{
	return m_impl->start(i_function);
}
fiber::stop_result fiber::stop()
{
	return m_impl->stop();
}
bool fiber::ready() const
{
	return m_impl->get_state() == FiberExecutionState::Idle;
}
fiber_id fiber::get_id() const
{
	return m_impl->get_id();
}
bool fiber::joinable() const
{
	return m_impl->joinable();
}

}