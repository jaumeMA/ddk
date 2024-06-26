
#include <thread>
#include "ddk_reference_wrapper.h"
#include "ddk_thread_impl.h"
#include "ddk_exception_handler.h"
#include "ddk_async_exceptions.h"

extern "C"
{

void consolidate_frame();

}

namespace ddk
{
namespace detail
{

template<typename T>
typed_yielder_context<T>* this_fiber_t::get_typed_context() const
{
	return m_execContext.template get_typed_context<typed_yielder_context<T>>();
}

template<typename Return>
void launch_fiber(const ddk::function<Return()>* i_function, fiber_impl* i_fiber)
{
	if(i_fiber->m_state == FiberExecutionState::Executing)
	{
		const ddk::function<Return()> localCallable = *i_function;

		exception_handler::open_scope([callable = localCallable]()
		{
			if constexpr (std::is_same<Return,void>::value)
			{
				eval(callable);
			}
			else
			{
				yield(eval(callable));
			}
		}).on_error([](const fiber_error& i_error)
		{
			switch (i_error.get_error())
			{
				case FiberErrorCode::Suspended:
				{
					//do nothing, just exit
					break;
				}
				case FiberErrorCode::AsyncExecption:
				default:
				{
					yield(async_exception{ i_error.what(),AsyncExceptionCode::Cancelled });

					break;
				}
			}
		});
	}

	i_fiber->m_state = FiberExecutionState::Done;
}

template<typename Return>
void fiber_impl::start_from(this_fiber_t& other, const ddk::function<Return()>& i_function)
{
	m_state = FiberExecutionState::Executing;

	other.attach_context();

	m_fiberContext.attach_stack(m_alloc.allocate());

	make_execution_context(m_fiberContext,other.get_execution_context(),&consolidate_frame,&i_function,this,&launch_fiber<Return>);
}

}
}
