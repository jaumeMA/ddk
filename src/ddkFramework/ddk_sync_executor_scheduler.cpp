#include "ddk_sync_executor_scheduler.h"

namespace ddk
{

polling_async_scheduler::polling_async_scheduler(thread i_thread,std::chrono::milliseconds i_sleepTimeInMS)
: m_executor(std::move(i_thread),i_sleepTimeInMS)
{
}

}