#include "ddk_thread_sheaf.h"

namespace ddk
{

thread_sheaf::thread_sheaf(thread_sheaf&& other)
: m_threadCtr(std::move(other.m_threadCtr))
{
}
void thread_sheaf::clear()
{
	m_threadCtr.clear();
}
thread thread_sheaf::extract()
{
	if(m_threadCtr.empty() == false)
	{
		thread_container::iterator itThread = m_threadCtr.begin();

		thread firstThread = std::move(*itThread);

		m_threadCtr.erase(itThread);

		return std::move(firstThread);
	}
	else
	{
		EWAS_FAIL("Trying to pop empty thread sheaf");

		return crash_on_return<thread&&>::value();
	}
}
thread_sheaf::iterator thread_sheaf::begin()
{
	return m_threadCtr.begin();
}
thread_sheaf::iterator thread_sheaf::end()
{
	return m_threadCtr.end();
}
thread_sheaf::const_iterator thread_sheaf::begin() const
{
	return m_threadCtr.begin();
}
thread_sheaf::const_iterator thread_sheaf::end() const
{
	return m_threadCtr.end();
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