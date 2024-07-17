#include "ddk_sync_executor_scheduler.h"

namespace ddk
{

chained_async_scheduler::chained_async_scheduler(detail::private_async_state_base_shared_ref i_sharedState)
: m_sharedState(i_sharedState)
{
}

polling_async_scheduler::polling_async_scheduler(thread i_thread,std::chrono::milliseconds i_sleepTimeInMS)
: m_executor(std::move(i_thread),i_sleepTimeInMS)
{
}

}