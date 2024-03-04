
namespace ddk
{
namespace detail
{

template<typename Callable>
result<void,iterable_error> iterable_exception_handler::open_scope(Callable&& i_callable)
{
	return s_iterable_exception_handler.open_scope(std::forward<Callable>(i_callable));
}
template<typename ... Args>
void iterable_exception_handler::close_scope(Args&& ... i_args)
{
	s_iterable_exception_handler.close_scope(std::forward<Args>(i_args)...);
}

}
}