#include "ddk_sync_executor_impl.h"
#include "ddk_async_exceptions.h"

namespace ddk
{
namespace detail
{

executor_context_lent_ptr immediate_executor::get_execution_context()
{
	return lend(m_execContext);
}
executor_context_const_lent_ptr immediate_executor::get_execution_context() const
{
	return lend(m_execContext);
}

fiber_executor::fiber_executor(fiber i_fiber)
: m_execContext(std::move(i_fiber))
{
}
executor_context_lent_ptr fiber_executor::get_execution_context()
{
	return lend(m_execContext);
}
executor_context_const_lent_ptr fiber_executor::get_execution_context() const
{
	return lend(m_execContext);
}

thread_executor::thread_executor(thread i_thread)
: m_execContext(std::move(i_thread))
, m_state(ExecutorState::Idle)
{
}
executor_context_lent_ptr thread_executor::get_execution_context()
{
	return lend(m_execContext);
}
executor_context_const_lent_ptr thread_executor::get_execution_context() const
{
	return lend(m_execContext);
}

thread_sheaf_executor::thread_sheaf_executor(thread_sheaf i_threadSheaf)
: m_execContext(std::move(i_threadSheaf))
, m_state(ExecutorState::Idle)
{
}
executor_context_lent_ptr thread_sheaf_executor::get_execution_context()
{
	return lend(m_execContext);
}
executor_context_const_lent_ptr thread_sheaf_executor::get_execution_context() const
{
	return lend(m_execContext);
}

fiber_sheaf_executor::fiber_sheaf_executor(fiber_sheaf i_fiberSheaf)
: m_execContext(std::move(i_fiberSheaf))
, m_state(ExecutorState::Idle)
{
}
executor_context_lent_ptr fiber_sheaf_executor::get_execution_context()
{
	return lend(m_execContext);
}
executor_context_const_lent_ptr fiber_sheaf_executor::get_execution_context() const
{
	return lend(m_execContext);
}

execution_context_executor::execution_context_executor(executor_context_lent_ptr i_execContext,unsigned char i_depth)
: m_execContext(i_execContext)
, m_state(ExecutorState::Idle)
, m_depth(i_depth)
{
}
executor_context_lent_ptr execution_context_executor::get_execution_context()
{
	return m_execContext;
}
executor_context_const_lent_ptr execution_context_executor::get_execution_context() const
{
	return m_execContext;
}

}
}
