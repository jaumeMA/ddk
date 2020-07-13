#include "ddk_fiber_sheaf.h"

namespace ddk
{

fiber_sheaf::fiber_sheaf(fiber_sheaf&& other)
: m_fiberCtr(std::move(other.m_fiberCtr))
{
}
fiber_sheaf::iterator fiber_sheaf::begin()
{
	return m_fiberCtr.begin();
}
fiber_sheaf::iterator fiber_sheaf::end()
{
	return m_fiberCtr.end();
}
fiber_sheaf::const_iterator fiber_sheaf::begin() const
{
	return m_fiberCtr.begin();
}
fiber_sheaf::const_iterator fiber_sheaf::end() const
{
	return m_fiberCtr.end();
}
void fiber_sheaf::clear()
{
	m_fiberCtr.clear();
}
fiber fiber_sheaf::extract()
{
	if(m_fiberCtr.empty() == false)
	{
		fiber_container::iterator itFiber = m_fiberCtr.begin();

		fiber newFiber = std::move(*itFiber);
	
		m_fiberCtr.erase(itFiber);

		return std::move(newFiber);
	}
	else
	{
		DDK_FAIL("Trying to pop fiber from empty sheaf");

		return ddk::crash_on_return<fiber&&>::value();
	}
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