#include "ddk_execution_model.h"

namespace ddk
{
namespace detail
{

polling_execution_model::polling_execution_model(const std::chrono::milliseconds& i_sleepInMs)
: m_sleepTimeInMS(i_sleepInMs)
, m_stopped(true)
{
}
void polling_execution_model::resume()
{
	m_stopped = true;
}

async_execution_model::async_execution_model(const std::chrono::milliseconds& i_sleepInMs)
: m_sleepTimeInMS(i_sleepInMs)
, m_stopped(true)
, m_pendingWork(false)
, m_condVarMutex(MutexType::Recursive)
{
}
void async_execution_model::signal()
{
	mutex_guard mg(m_condVarMutex);

	m_pendingWork = true;

	m_condVar.notify_one();
}
void async_execution_model::resume()
{
	m_condVarMutex.lock();

	if (m_stopped == false)
	{
		m_stopped = true;

		m_condVar.notify_all();

		m_condVarMutex.unlock();
	}
	else
	{
		m_condVarMutex.unlock();
	}
}

void forward_execution_model::resume()
{
}

}
}