#include "ddk_this_thread.h"
#include "ddk_thread_utils.h"
#include "ddk_fiber_utils.h"

namespace ddk
{

namespace detail
{

this_thread_t::this_thread_t()
: m_execContext(fiber_id(static_cast<size_t>(get_current_thread_id())))
{
	set_current_execution_context(m_execContext);
}
void this_thread_t::attach_context() const
{
	set_current_execution_context(m_execContext);
}

}
}
