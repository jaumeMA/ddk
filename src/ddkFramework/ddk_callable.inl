
namespace ddk
{
namespace detail
{

template<typename Return,typename Function,bool Terse>
fixed_return_callable_impl<Return,Function,Terse>::fixed_return_callable_impl(const Function& i_function)
: m_function(i_function)
{
}
template<typename Return,typename Function,bool Terse>
fixed_return_callable_impl<Return,Function,Terse>::fixed_return_callable_impl(Function&& i_function)
: m_function(std::move(i_function))
{
}
template<typename Return,typename Function,bool Terse>
template<typename ... Args>
Return fixed_return_callable_impl<Return,Function,Terse>::operator()(Args&& ... i_args) const
{
	if constexpr (Terse)
	{
		return ddk::terse_eval(std::move(m_function),std::forward<Args>(i_args)...);
	}
	else
	{
		return ddk::eval(std::move(m_function),std::forward<Args>(i_args)...);
	}
}

template<typename Return,typename Function,bool Terse>
template<typename ... Args>
replaced_return_callable_impl<Return,Function,Terse>::replaced_return_callable_impl(const Function& i_function,Args&& ... i_args)
: m_function(i_function)
, m_return(std::forward<Args>(i_args)...)
{
}
template<typename Return,typename Function,bool Terse>
template<typename ... Args>
replaced_return_callable_impl<Return,Function,Terse>::replaced_return_callable_impl(Function&& i_function,Args&& ... i_args)
: m_function(std::move(i_function))
, m_return(std::forward<Args>(i_args)...)
{
}
template<typename Return,typename Function,bool Terse>
template<typename ... Args>
Return replaced_return_callable_impl<Return,Function,Terse>::operator()(Args&& ... i_args) const
{
	if constexpr (Terse)
	{
		ddk::terse_eval(std::move(m_function),std::forward<Args>(i_args)...);
	}
	else
	{
		ddk::eval(std::move(m_function),std::forward<Args>(i_args)...);
	}

	return std::move(m_return);
}

}
}