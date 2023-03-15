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
thread_sheaf::start_result thread_sheaf::start(const ddk::function<void()>& i_function)
{
	size_t threadIndex = 0;

	for(auto& thread : m_threadCtr)
	{
		if(auto startRes = thread.start(i_function))
		{
			++threadIndex;
		}
	}

	return ddk::make_result<start_result>(threadIndex);
}
thread_sheaf::start_result thread_sheaf::start(const ddk::function<void()>& i_function, size_t i_numThreads)
{
	size_t threadIndex=0;

	for(auto& thread : m_threadCtr)
	{
		if(threadIndex >= i_numThreads)
			break;

		if(auto startRes = thread.start(i_function))
		{
			++threadIndex;
		}
	}

	return ddk::make_result<start_result>(threadIndex);
}
thread_sheaf::stop_result thread_sheaf::stop()
{
	size_t threadIndex = 0;

	for(auto& thread : m_threadCtr)
	{
		if(auto stopRes = thread.stop())
		{
			++threadIndex;
		}
	}

	return ddk::make_result<stop_result>(threadIndex);
}
bool thread_sheaf::joinable() const
{
	for(auto& thread : m_threadCtr)
	{
		if(thread.joinable())
		{
			return true;
		}
	}

	return false;
}
size_t thread_sheaf::set_affinity(const cpu_set_t& i_set)
{
	size_t res = 0;

	for(auto& thread : m_threadCtr)
	{
		if(thread.set_affinity(i_set))
		{
			++res;
		}
	}

	return res;
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

thread_sheaf& operator<<(thread_sheaf& i_lhs, thread i_rhs)
{
	i_lhs.m_threadCtr.emplace_back(std::move(i_rhs));

	return i_lhs;
}

}