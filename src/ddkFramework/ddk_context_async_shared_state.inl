
namespace ddk
{
namespace detail
{

template<typename T>
TEMPLATE(typename ... Args)
REQUIRED(IS_CONSTRUCTIBLE(T,Args...))
context_private_async_state<T>::context_private_async_state(Args&& ... i_args)
: private_async_state<T>(std::forward<Args>(i_args)...)
{
}

}
}