#include "ddk_fiber_utils.h"
#include "ddk_thread_utils.h"
#include "ddk_thread_impl.h"
#include "ddk_reference_wrapper.h"
#include "ddk_async_exceptions.h"
#include "ddk_fiber_exception_handler.h"

namespace ddk
{
namespace detail
{

thread_local execution_context* __s_current_execution_context = nullptr;

}

fiber_id get_current_fiber_id()
{
	detail::execution_context* currFiberContext = detail::__s_current_execution_context;

	return currFiberContext->get_id();
}

void suspend()
{
    detail::fiber_exception_handler::close_scope(FiberErrorCode::Suspended,suspend_exception{ get_current_fiber_id() });
}
void suspend(int i_code,const std::string& i_reason)
{
    detail::fiber_exception_handler::close_scope(FiberErrorCode::Suspended,suspend_exception{ get_current_fiber_id(),i_code,i_reason});
}
void yield()
{
    if(ddk::detail::execution_context* currFiberContext = get_current_execution_context())
    {
        if(ddk::detail::yielder_interface* currYielder = currFiberContext->get_yielder())
        {
            ddk::detail::typed_yielder_context<detail::void_t> _yielder(_void);

            currYielder->yield(&_yielder);

            if(currFiberContext->is_stopped())
            {
                currFiberContext->m_excpHandler.close_scope(FiberErrorCode::Suspended,suspend_exception{ currFiberContext->get_id() });
            }
        }
        else
        {
            std::this_thread::yield();
        }
    }
    else
    {
        std::this_thread::yield();
    }
}

void pause()
{
    if(ddk::detail::execution_context* currFiberContext = get_current_execution_context())
    {
        if(ddk::detail::yielder_interface* currYielder = currFiberContext->get_yielder())
        {
            ddk::detail::yielder_context _yielder(get_current_fiber_id(),ddk::detail::yielder_context::Paused);

            currYielder->yield(&_yielder);

            if(currFiberContext->is_stopped())
            {
                currFiberContext->m_excpHandler.close_scope(FiberErrorCode::Suspended,suspend_exception{ currFiberContext->get_id() });
            }
        }
        else
        {
            std::this_thread::yield();
        }
    }
    else
    {
        std::this_thread::yield();
    }
}

}
