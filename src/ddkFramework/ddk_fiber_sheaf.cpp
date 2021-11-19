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
void fiber_sheaf::start(const ddk::function<void()>& i_function)
{
	fiber_container::iterator itFiber = m_fiberCtr.begin();
	for (; itFiber != m_fiberCtr.end(); ++itFiber)
	{
		itFiber->start(i_function);
	}
}
void fiber_sheaf::stop()
{
	fiber_container::iterator itFiber = m_fiberCtr.begin();
	for (; itFiber != m_fiberCtr.end(); ++itFiber)
	{
		itFiber->stop();
	}
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