
#include "ddk_async.h"

namespace ddk
{

template<typename Executor, typename Allocator,typename Callable,typename Provider, typename ... Args>
auto async(delayed_task<Callable>&& i_task, Provider& i_eventProvider, Args&& ... i_args)
{
	return ddk::async<Allocator>(std::move(i_task.m_executor),std::forward<Args>(i_args)...) -> at<event_driven_async_scheduler<Provider>>(lend(i_eventProvider))
   																							 -> attach<detail::on_time_context_executor<Executor>>();
}

template<typename Callable>
delayed_task<Callable>::delayed_task(const Callable& i_callable)
: m_executor(i_callable)
{
}
template<typename Callable>
delayed_task<Callable>::delayed_task(Callable&& i_callable)
: m_executor(std::move(i_callable))
{
}

}