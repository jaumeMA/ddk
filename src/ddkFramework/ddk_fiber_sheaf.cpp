#include "ddk_fiber_sheaf.h"

namespace ddk
{

fiber_sheaf::fiber_sheaf(fiber_sheaf&& other)
: m_fiberCtr(std::move(other.m_fiberCtr))
{
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
void fiber_sheaf::insert(fiber i_fiber)
{
	m_fiberCtr.push_back(std::move(i_fiber));
}
optional<fiber> fiber_sheaf::extract()
{
	fiber_container::iterator itFiber = m_fiberCtr.begin();
	for (; itFiber != m_fiberCtr.end(); ++itFiber)
	{
		if (itFiber->joinable() == false)
		{
			fiber extractedFiber = std::move(*itFiber);

			m_fiberCtr.erase(itFiber);

			return std::move(extractedFiber);
		}
	}

	return none;
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