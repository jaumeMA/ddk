#include "ddk_fiber_sheaf.h"

namespace ddk
{

fiber_sheaf::fiber_sheaf(fiber_sheaf&& other)
: m_fiberCtr(std::move(other.m_fiberCtr))
{
}
fiber_sheaf& fiber_sheaf::operator=(fiber_sheaf&& other)
{
	m_fiberCtr = std::move(other.m_fiberCtr);

	return *this;
}
fiber_sheaf::start_result fiber_sheaf::start(const ddk::function<void()>& i_function)
{
	size_t fiberIndex = 0;

	fiber_container::iterator itFiber = m_fiberCtr.begin();
	for (; itFiber != m_fiberCtr.end(); ++itFiber)
	{
		if(itFiber->start(i_function))
		{
			++fiberIndex;
		}
	}

	return ddk::make_result<start_result>(fiberIndex);
}
fiber_sheaf::stop_result fiber_sheaf::stop()
{
	size_t fiberIndex = 0;

	fiber_container::iterator itFiber = m_fiberCtr.begin();
	for (; itFiber != m_fiberCtr.end(); ++itFiber)
	{
		if(itFiber->stop())
		{
			++fiberIndex;
		}
	}

	return ddk::make_result<stop_result>(fiberIndex);
}
bool fiber_sheaf::joinable() const
{
	fiber_container::const_iterator itFiber = m_fiberCtr.begin();
	for(; itFiber != m_fiberCtr.end(); ++itFiber)
	{
		if(itFiber->joinable())
		{
			return true;
		}
	}

	return false;
}
void fiber_sheaf::clear()
{
	m_fiberCtr.clear();
}
bool fiber_sheaf::empty() const
{
	return m_fiberCtr.empty();
}
size_t fiber_sheaf::size() const
{
	return m_fiberCtr.size();
}

}