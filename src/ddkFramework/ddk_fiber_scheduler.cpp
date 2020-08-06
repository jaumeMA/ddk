#include "ddk_fiber_scheduler.h"

namespace ddk
{
namespace detail
{

running_fiber::running_fiber(detail::fiber_impl* i_fiberImpl, time_t i_time)
: m_fiber(i_fiberImpl)
, m_lastExecutionTime(i_time)
, m_totalExecution(0)
{
}
void running_fiber::set_last_time(time_t i_time)
{
	m_totalExecution += i_time - m_lastExecutionTime;

	m_lastExecutionTime = i_time;
}
time_t running_fiber::get_last_time() const
{
	return m_lastExecutionTime;
}
time_t running_fiber::get_total_duration_time() const
{
	return m_totalExecution;
}
size_t running_fiber::get_num_executions() const
{
	return m_numOfExecutions;
}
running_fiber& running_fiber::operator++()
{
	m_numOfExecutions++;

	return *this;
}
bool running_fiber::operator==(fiber_id i_id) const
{
	return i_id == m_fiber->get_id();
}
bool running_fiber::operator<(const running_fiber& other) const
{
	return m_lastExecutionTime < other.m_lastExecutionTime;
}
detail::fiber_impl* running_fiber::operator->()
{
	return m_fiber;
}
const detail::fiber_impl* running_fiber::operator->() const
{
	return m_fiber;
}

}

bool running_fiber_comparator::operator()(const detail::running_fiber& i_lhs, detail::running_fiber& i_rhs) const
{
	//default impl
	return i_lhs.get_last_time() > i_rhs.get_last_time();
}

}