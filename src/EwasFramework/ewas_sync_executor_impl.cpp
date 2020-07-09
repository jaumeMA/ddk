#include "ewas_sync_executor_impl.h"

namespace ewas
{
namespace detail
{

thread_sheaf_executor::thread_sheaf_executor(thread_sheaf i_threadSheaf)
: m_threadSheaf(std::move(i_threadSheaf))
, m_pendingThreads(m_threadSheaf.size())
{}
thread_sheaf_executor::start_result thread_sheaf_executor::execute(const std::function<void(detail::void_t)>& i_sink, const std::function<detail::void_t()>& i_callable)
{
	thread_sheaf::iterator itThread = m_threadSheaf.begin();
	for(;itThread!=m_threadSheaf.end();++itThread)
	{
		itThread->start([this,i_sink,i_callable](){ i_callable(); --m_pendingThreads; if(m_pendingThreads == 0) i_sink(_void); });
	}

	return make_result<start_result>(ExecutorState::Executed);
}
ExecutorState thread_sheaf_executor::get_state() const
{
	if(m_pendingThreads == 0)
	{
		return ExecutorState::Executed;
	}
	else
	{
		thread_sheaf::iterator itThread = m_threadSheaf.begin();
		for(;itThread!=m_threadSheaf.end();++itThread)
		{
			if(itThread->joinable())
			{
				return ExecutorState::Executed;
			}
		}

		return ExecutorState::Idle;
	}
}

fiber_sheaf_executor::fiber_sheaf_executor(fiber_sheaf i_fiberSheaf)
: m_fiberSheaf(std::move(i_fiberSheaf))
, m_pendingFibers(m_fiberSheaf.size())
{
}
fiber_sheaf_executor::start_result fiber_sheaf_executor::execute(const std::function<void(detail::void_t)>& i_sink, const std::function<detail::void_t()>& i_callable)
{
	fiber_sheaf::iterator itFiber = m_fiberSheaf.begin();
	for(;itFiber!=m_fiberSheaf.end();++itFiber)
	{
		itFiber->start([this,i_sink,i_callable]()
		{
			try
			{
				i_callable(); 
			}
			catch(const suspend_exception&)
			{
			}

			--m_pendingFibers;
			if(m_pendingFibers == 0)
			{
				i_sink(_void);
			}
		});
	}

	return make_result<start_result>(ExecutorState::Executed);
}
ExecutorState fiber_sheaf_executor::get_state() const
{
	if(m_pendingFibers == 0)
	{
		return ExecutorState::Executed;
	}
	else
	{
		fiber_sheaf::iterator itFiber = m_fiberSheaf.begin();
		for(;itFiber!=m_fiberSheaf.end();++itFiber)
		{
			if(itFiber->ready() == false)
			{
				return ExecutorState::Executed;
			}
		}

		return ExecutorState::Idle;
	}
}

}
}