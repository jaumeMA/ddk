#include "ddk_thread_sheaf.h"

namespace ddk
{

thread_sheaf::thread_sheaf(thread_sheaf&& other)
: m_threadCtr(std::move(other.m_threadCtr))
{
}
thread_sheaf& thread_sheaf::operator=(thread_sheaf&& other)
{
	m_threadCtr = std::move(other.m_threadCtr);

	return *this;
}
void thread_sheaf::start(const ddk::function<void()>& i_function)
{
	thread_container::iterator itThread = m_threadCtr.begin();
	for (; itThread != m_threadCtr.end(); ++itThread)
	{
		if (itThread->joinable() == false)
		{
			itThread->start(i_function);
		}
	}
}
size_t thread_sheaf::start(const ddk::function<void()>& i_function, size_t i_numThreads)
{
	size_t threadIndex=0;
	thread_container::iterator itThread = m_threadCtr.begin();
	for (; threadIndex < i_numThreads && itThread != m_threadCtr.end(); ++itThread)
	{
		if (itThread->joinable() == false)
		{
			itThread->start(i_function);
			++threadIndex;
		}
	}

	return threadIndex;
}
void thread_sheaf::stop()
{
	thread_container::iterator itThread = m_threadCtr.begin();
	for (; itThread != m_threadCtr.end(); ++itThread)
	{
		itThread->stop();
	}
}
bool thread_sheaf::joinable() const
{
	thread_container::const_iterator itThread = m_threadCtr.begin();
	for(; itThread != m_threadCtr.end(); ++itThread)
	{
		if(itThread->joinable())
		{
			return true;
		}
	}

	return false;
}
thread_sheaf::iterator thread_sheaf::begin()
{
	return m_threadCtr.begin();
}
thread_sheaf::const_iterator thread_sheaf::begin() const
{
	return m_threadCtr.begin();
}
thread_sheaf::iterator thread_sheaf::end()
{
	return m_threadCtr.end();
}
thread_sheaf::const_iterator thread_sheaf::end() const
{
	return m_threadCtr.end();
}
void thread_sheaf::clear()
{
	m_threadCtr.clear();
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