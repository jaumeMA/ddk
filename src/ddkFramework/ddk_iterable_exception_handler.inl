
namespace ddk
{
namespace detail
{

template<typename Callable>
constexpr iterable_result exception_handler<iterable_exception_handler_t>::create_context(Callable&& i_callable)
{
	exception_handler<iterable_exception_handler_t>& thisHandler = get();

	if (setjmp(thisHandler.m_context) == 0)
	{
		i_callable();

		return make_result<iterable_result>(success);
	}
	else if(thisHandler.m_type == detail::iterable_exception_handler::Terminated)
	{
		return make_result<iterable_result>(success);
	}
	else
	{
		return make_error<iterable_result>(IterableError::UserError,thisHandler.m_reason);
	}
}

}
}