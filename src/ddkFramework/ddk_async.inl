#include "ddk_sync_executor.inl"

namespace ddk
{

template<typename Callable>
constexpr auto async(Callable&& i_callable)
{
	return async<system_allocator>(std::forward<Callable>(i_callable));
}
template<typename Allocator,typename Callable,typename ... Args>
constexpr auto async(Callable&& i_callable,Args&& ... i_args)
{
	typedef typename mpl::aqcuire_callable_return_type<Callable>::type callable_return_type;

	if constexpr (mpl::is_void<callable_return_type>)
	{
		auto voidCallable = [callable = std::forward<Callable>(i_callable)]() mutable
		{
			eval(std::forward<Callable>(callable));

			return _void;
		};

		return make_async_executor(std::move(voidCallable),[]() { return false; },executor_promise<detail::void_t,Allocator>{std::forward<Args>(i_args)...});
	}
	else
	{
		return make_async_executor(std::forward<Callable>(i_callable),[]() { return false; },executor_promise<callable_return_type,Allocator>{std::forward<Args>(i_args)...});
	}
}

}