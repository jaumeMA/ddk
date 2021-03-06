#include "ddk_fiber.h"

namespace ddk
{
namespace detail
{

}

//const thread_local detail::this_fiber_t this_fiber = detail::this_fiber_t{};

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
void fiber::start(const ddk::function<void()>& i_function)
{
	m_impl->start(i_function);
}
void fiber::stop()
{
	m_impl->stop();
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