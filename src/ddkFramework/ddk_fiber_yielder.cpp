#include "ddk_fiber_yielder.h"
#include "ddk_fiber_scheduler_interface.h"
#include "ddk_thread_yielder.h"

namespace ddk
{
namespace detail
{

yielder::yielder(yielder_interface& i_executor)
: m_executor(i_executor)
{
}

void yielder::yield(yielder_context* i_context)
{
    m_executor.yield(i_context);
}
void yielder::suspend(yielder_context* i_context)
{
    m_executor.suspend(i_context);
}


}
}
