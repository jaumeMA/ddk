
namespace ddk
{
namespace detail
{

template<typename Callable>
callable_impl<Callable>::callable_impl(const Callable& i_callable)
: m_callable(i_callable)
{
}
template<typename Callable>
typename callable_impl<Callable>::return_type callable_impl<Callable>::operator()() const
{
	if constexpr (std::is_void<return_type>::value)
	{
		eval(m_callable);
	}
	else
	{
		return eval(m_callable);
	}
}
template<typename Callable>
typename callable_impl<Callable>::return_type callable_impl<Callable>::operator()()
{
	if constexpr(std::is_void<return_type>::value)
	{
		eval(m_callable);
	}
	else
	{
		return eval(m_callable);
	}
}

}

template<typename Return>
template<typename Callable>
type_erased_callable<Return>::type_erased_callable(Callable& i_callable)
{
	m_callable = new detail::callable_impl<Callable>(i_callable);
}
template<typename Return>
Return type_erased_callable<Return>::operator()() const
{
	if constexpr(std::is_void<Return>::value)
	{
		(*m_callable)();
	}
	else
	{
		return (*m_callable)();
	}
}
template<typename Return>
Return type_erased_callable<Return>::operator()()
{
	if constexpr(std::is_void<Return>::value)
	{
		(*m_callable)();
	}
	else
	{
		return (*m_callable)();
	}
}


template<typename Callable>
auto make_callable(Callable& i_callable)
{
	typedef typename mpl::aqcuire_callable_return_type<Callable>::type return_type;

	return type_erased_callable<return_type>{i_callable};
}

}