#include "ddk_thread_sheaf.h"

namespace ddk
{

thread_sheaf::thread_sheaf(thread_sheaf&& other)
: m_threadCtr(std::move(other.m_threadCtr))
{
}
void thread_sheaf::start(const std::function<void()>& i_function)
{
	thread_container::iterator itThread = m_threadCtr.begin();
	for (; itThread != m_threadCtr.end(); ++itThread)
	{
		itThread->start(i_function);
	}
}
void thread_sheaf::stop()
{
	thread_container::iterator itThread = m_threadCtr.begin();
	for (; itThread != m_threadCtr.end(); ++itThread)
	{
		itThread->stop();
	}
}
void thread_sheaf::insert(thread i_thread)
{
	m_threadCtr.push_back(std::move(i_thread));
}
optional<thread> thread_sheaf::extract()
{
	thread_container::iterator itThread = m_threadCtr.begin();
	for (; itThread != m_threadCtr.end(); ++itThread)
	{
		if (itThread->joinable() == false)
		{
			thread extractedThread = std::move(*itThread);

			m_threadCtr.erase(itThread);

			return std::move(extractedThread);
		}
	}

	return none;
}
size_t thread_sheaf::size() const
{
	return m_threadCtr.size();
}
bool thread_sheaf::empty()
{
	return m_threadCtr.empty();
}

}