#include "ddk_sync_executor_scheduler.h"

namespace ddk
{

void null_async_scheduler::clear_scheduler()
{
}

polling_async_scheduler::polling_async_scheduler(thread i_thread,std::chrono::milliseconds i_sleepTimeInMS)
: m_thread(std::move(i_thread))
, m_sleepTimeInMS(i_sleepTimeInMS)
{
}
polling_async_scheduler::~polling_async_scheduler()
{
	if (m_stopped == false)
	{
		m_stopped = true;

		m_thread.stop();
	}
}

}