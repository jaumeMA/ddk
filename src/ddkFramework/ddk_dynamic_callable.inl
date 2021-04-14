
namespace ddk
{

template<typename Callable,typename Return,typename TypeInterface>
dynamic_callable<Callable,Return,TypeInterface>::dynamic_callable(const Callable& i_callable)
: m_callable(i_callable)
{
}
template<typename Callable,typename Return,typename TypeInterface>
dynamic_callable<Callable,Return,TypeInterface>::dynamic_callable(Callable&& i_callable)
: m_callable(std::move(i_callable))
{
}
template<typename Callable,typename Return,typename TypeInterface>
template<typename ... Args>
Return dynamic_callable<Callable,Return,TypeInterface>::operator()(Args&& ... i_args)
{
	if constexpr (std::is_same<Return,void>::value)
	{
		eval(m_callable,std::forward<Args>(i_args)...);
	}
	else
	{
		return eval(m_callable,std::forward<Args>(i_args)...);
	}
}

}