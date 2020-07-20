#include "ddk_thread_yielder.h"

namespace ddk
{
namespace detail
{

yielder_context::yielder_context()
{
	m_stop = std::make_pair(ddk::fiber_id(0),false);
}
void yielder_context::stop(fiber_id i_id)
{
	m_stop = std::make_pair(i_id,true);
}
bool yielder_context::is_stopped(fiber_id i_id) const
{
	return m_stop.first == i_id && m_stop.second;
}

}
}